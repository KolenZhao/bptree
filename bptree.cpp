#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

NodeLocation::NodeLocation(BPTreeNode* ptr, int i) {
	this->ptr = ptr;
	this->i = i;
}

NodeLocation BPTreeNode::minNode() {
	if (this->leaf) {
		return NodeLocation(this, 0);
	} else {
		return ((InterNode*)this)->child[0]->minNode();
	}
}

NodeLocation BPTreeNode::maxNode() {
	if (this->leaf) {
		return NodeLocation(this, this->n-1);
	} else {
		return ((InterNode*)this)->child[this->n]->maxNode();
	}
}

NodeLocation BPTreeNode::precursor(int keyIndex) {
	return ((InterNode*)this)->child[keyIndex]->maxNode();
}

NodeLocation BPTreeNode::successor(int keyIndex) {
	return ((InterNode*)this)->child[keyIndex+1]->minNode();
}

int BPTreeNode::mergeChild(int keyIndex) {
	InterNode* s = (InterNode*)this;
	BPTreeNode* y = s->child[keyIndex];
	BPTreeNode* z = s->child[keyIndex+1];

	if (y->leaf) {
		LeafNode* yy = (LeafNode*)y;
		LeafNode* zz = (LeafNode*)z;
		for (int i = 0; i < MINKEYS; i++) {
			y->keys[y->n] = z->keys[i];
			yy->values[y->n] = zz->values[i];
			y->n++;
		}
		yy->next = zz->next;
		delete(z);
		s->simpleRemoveRight(keyIndex);
	}
	else {
		InterNode* yy = (InterNode*)y;
		InterNode* zz = (InterNode*)z;
		y->keys[y->n] = this->keys[keyIndex];
		y->n++;

		for (int i = 0; i < MINKEYS; i++) {
			y->keys[y->n] = z->keys[i];
			yy->child[y->n] = zz->child[i];
			y->n++;
		}
		yy->child[y->n] = zz->child[z->n];
		delete(z);
		s->simpleRemoveRight(keyIndex);
	}
	return 0;
}

InterNode::InterNode(int n) {
	this->n = n;
	this->leaf = false;
}

int InterNode::simpleInsertRight(int index, int key, BPTreeNode* ptr) {
	for (int i = this->n - 1; i >= index; i--) {
		this->keys[i + 1] = this->keys[i];
		this->child[i + 2] = this->child[i + 1];
	}
	this->keys[index] = key;
	this->child[index + 1] = ptr;
	this->n += 1;
	return 0;
}

int InterNode::simpleInsertLeft(int index, int key, BPTreeNode* ptr) {
	for (int i=this->n-1; i >= index; i--) {
		this->keys[i+1] = this->keys[i];
		this->child[i+2] = this->child[i+1];
	}
	this->child[index+1] = this->child[index];

	this->keys[index] = key;
	this->child[index] = ptr;
	this->n += 1;
	return 0;
}

int InterNode::simpleRemoveRight(int index) {
	for (int i = index + 1; i < this->n; i++) {
		this->keys[i - 1] = this->keys[i];
		this->child[i] = this->child[i + 1];
	}
	this->n -= 1;
	return 0;
}

int InterNode::simpleRemoveLeft(int index) {
	for (int i = index + 1; i < this->n; i++) {
		this->keys[i - 1] = this->keys[i];
		this->child[i - 1] = this->child[i];
	}
	this->child[this->n - 1] = this->child[this->n];
	this->n -= 1;
	return 0;
}


LeafNode::LeafNode(int n) {
	this->n = n;
	this->leaf = true;
	this->next = NULL;
}

int LeafNode::simpleInsert(int index, int key, int value) {
	for (int i = this->n - 1; i >= index; i--) {
		this->keys[i + 1] = this->keys[i];
		this->values[i + 1] = this->values[i];
	}
	this->keys[index] = key;
	this->values[index] = value;
	this->n += 1;
	return 0;
}

int LeafNode::leafRemove(int index) {
	for (int i = index + 1; i < this->n; i++) {
		this->keys[i - 1] = this->keys[i];
		this->values[i - 1] = this->values[i];
	}
	this->n -= 1;
	return 0;
}

BPTreeNode* newNode(int n, bool leaf) {
	if (leaf) {
		return new LeafNode(n);
	}
	else {
		return new InterNode(n);
	}
}

int find(int *list, int length, int key) {
    int bot = 0;
    int top = length - 1;
    int mid = 0;
    while (bot <= top) {
        mid = (bot + top) / 2;
        if (list[mid] == key) {
            return mid;
        } else if (key > list[mid-1] && key <= list[mid]) {
            return mid;
        } else {
            if (list[mid] > key) {
                top = mid - 1;
            } else {
                bot = mid + 1;
            }
        }
    }
    return mid;
}

int BPTreeNode::remove(int key){
	if (this->n < 0) {
		printf("Erorr: this->n < 0.\n");
	}
	if (this->n == 0) {
		return 2;
	}
	int index = find(this->keys, this->n, key);
	if (this->keys[index] == key) {
		if (this->leaf) {
			((LeafNode*)this)->leafRemove(index);
		} else {
			InterNode* s = (InterNode*)this;
			if (s->child[index]->n >= MIN) {
				s->child[index]->remove(key);
				NodeLocation pre = this->precursor(index);
				this->keys[index] = pre.ptr->keys[pre.i];
			} else if (s->child[index+1]->n >= MIN) {
				NodeLocation suc = this->successor(index);
				this->keys[index] = suc.ptr->keys[suc.i];
				NodeLocation d = s->child[index]->getLocation(key);
				((LeafNode*)d.ptr)->keys[d.i] = suc.ptr->keys[suc.i];
				((LeafNode*)d.ptr)->values[d.i] = ((LeafNode*)suc.ptr)->values[suc.i];
				s->child[index + 1]->remove(suc.ptr->keys[suc.i]);
				//s->child[index]->insertNonFull(suc.ptr->keys[suc.i],
				//	((LeafNode*)suc.ptr)->values[suc.i]);
				//s->child[index]->remove(key);
			} else {
				this->mergeChild(index);
				s->child[index]->remove(key);
			}
		}
	} else {
		if (this->leaf) {
			return 1;
		}

		if (key > this->keys[index]) {
			index += 1;
		}
		InterNode* s = (InterNode*)this;
		if (s->child[index]->n >= MIN) {
			return s->child[index]->remove(key);
		} else {
			if (s->child[index]->leaf) {
				LeafNode* child = (LeafNode*)s->child[index];
				if (index > 0 && s->child[index - 1]->n >= MIN) {
					LeafNode* prev = (LeafNode*)s->child[index - 1];
					NodeLocation pKey = s->child[index - 1]->getLocation(s->keys[index - 1]);
					child->simpleInsert(0, this->keys[index - 1], ((LeafNode*)pKey.ptr)->values[pKey.i]);
					this->keys[index - 1] = prev->keys[prev->n - 2];
					prev->leafRemove(prev->n - 1);
				}
				else if (index < this->n && s->child[index + 1]->n >= MIN) {
					LeafNode* next = (LeafNode*)s->child[index + 1];
					child->simpleInsert(s->child[index]->n, next->keys[0], next->values[0]);
					this->keys[index] = next->keys[0];
					next->leafRemove(0);
				}
				else {
					if (index >= this->n) {
						index -= 1;
					}
					this->mergeChild(index);
				}
			}
			else {
				InterNode* child = (InterNode*)s->child[index];
				if (index > 0 && s->child[index - 1]->n >= MIN) {
					InterNode* prev = (InterNode*)s->child[index - 1];
					child->simpleInsertLeft(0, this->keys[index - 1], prev->child[prev->n]);
					this->keys[index - 1] = prev->keys[prev->n - 1];
					prev->simpleRemoveRight(prev->n - 1);
				}
				else if (index < this->n && s->child[index + 1]->n >= MIN) {
					InterNode* next = (InterNode*)s->child[index + 1];
					child->simpleInsertRight(s->child[index]->n, this->keys[index], next->child[0]);
					this->keys[index] = next->keys[0];
					next->simpleRemoveLeft(0);
				}
				else {
					if (index >= this->n) {
						index -= 1;
					}
					this->mergeChild(index);
				}
			}
			return s->child[index]->remove(key);
		}	
	}
	return 0;
}

int BPTreeNode::get(int key) {
	int index = -1;
	LeafNode* node = this->search(key, &index);
	if (!node) {
		return 0;
	} else {
		return node->values[index];
	}
}

NodeLocation BPTreeNode::getLocation(int key) {
	int index = -1;
	LeafNode* node = this->search(key, &index);
	return NodeLocation(node, index);
}

int BPTreeNode::walk() {
	for (LeafNode* leaf = this->first(); leaf != NULL; leaf = leaf->next) {
		for (int i = 0; i < leaf->n; i++) {
			printf("key: %d, value: %d.\n", leaf->keys[i], leaf->values[i]);
		}
	}
	return 0;
}

LeafNode* BPTreeNode::first() {
	if (!this->leaf) {
		return ((InterNode*)this)->child[0]->first();
	}
	else {
		return (LeafNode*)this;
	}
}

int BPTreeNode::size() {
	int sum = 0;
	for (LeafNode* leaf = this->first(); leaf != NULL; leaf = leaf->next) {
		sum += leaf->n;
	}
	return sum;
}

int BPTreeNode::trueSize() {
	int sum;
	if (!this->leaf) {
		sum = 0;
		for (int i = 0; i <= this->n; i++) {
			InterNode*s = (InterNode*)this;
			sum += s->child[i]->trueSize();
		}
	}
	else {
		sum = this->n;
	}
	return sum;
}

LeafNode* BPTreeNode::search(int key, int* index){
	int i = find(this->keys, this->n, key);
	if (this->leaf) {
		if (i < this->n && key == this->keys[i]) {
			*index = i;
			return (LeafNode*)this;
		} else {
			return NULL;
		}
	} else {
		if (key > this->keys[i]) {
			i += 1;
		}
		InterNode* s = (InterNode*)this;
		return s->child[i]->search(key, index);
	}
}


int BPTreeNode::insertNonFull(int key, int value) {
	int i = this->n - 1;
	if (this->leaf) {
		LeafNode* s = (LeafNode*)this;
		while (i >= 0 && key < this->keys[i]) {
			this->keys[i+1] = this->keys[i];
			s->values[i+1] = s->values[i];
			i -= 1;
		}
		this->keys[i+1] = key;
		s->values[i+1] = value;
		this->n += 1;
	} else {
		int i = find(this->keys, this->n, key);
		if (key > this->keys[i]) {
			i += 1;
		}
		InterNode* s = (InterNode*)this;
		if (s->child[i]->n == MAX - 1) {
			this->splitChild(i);
			if (key > this->keys[i]) {
				i += 1;
			}
		}
		s->child[i]->insertNonFull(key, value);
	}
	return 0;
}

int BPTreeNode::splitChild(int index) {
	InterNode* s = (InterNode*)this;
	BPTreeNode* y = s->child[index];
	BPTreeNode* z = newNode(MINKEYS, y->leaf);
	
	if (y->leaf) {
		y->n = MIN;
		LeafNode* yy = (LeafNode*)y;
		LeafNode* zz = (LeafNode*)z;
		for (int i = 0; i < MINKEYS; i++) {
			z->keys[i] = y->keys[i + MIN];
			zz->values[i] = yy->values[i + MIN];
		}
		zz->next = yy->next;
		yy->next = zz;
	}
	else {
		y->n = MINKEYS;
		InterNode* yy = (InterNode*)y;
		InterNode* zz = (InterNode*)z;
		for (int i = 0; i < MINKEYS; i++) {
			z->keys[i] = y->keys[i + MIN];
			zz->child[i] = yy->child[i + MIN];
		}
		zz->child[MINKEYS] = yy->child[MAX - 1];
	}
	s->simpleInsertRight(index, y->keys[MINKEYS], z);
	return 0;
}

BPTree::BPTree() {
	BPTreeNode* x = newNode(0, true);
	this->root = x;
}

int BPTree::insert(int key, int value) {
	BPTreeNode* root = this->root;
	int index = -1;
	LeafNode* node = this->root->search(key, &index);
	if (node) {
		node->values[index] = value;
	} else {
		if (root->n == MAX - 1) {
			InterNode* s = new InterNode(0);
			this->root = s;
			s->child[0] = root;
			s->splitChild(0);
			s->insertNonFull(key, value);
		} else {
			root->insertNonFull(key, value);
		}
	}
	return 0;
}

int BPTree::remove(int key) {
	int v = this->root->remove(key);
	if (this->root->n <= 0 && !this->root->leaf) {
		InterNode* r = (InterNode*)this->root;
		BPTreeNode* newRoot = r->child[0];
		delete(this->root);
		this->root = newRoot;
	}
	return v;
}
