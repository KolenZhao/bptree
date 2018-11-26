#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

NodeLocation::NodeLocation(BPTreeNode* ptr, int i) {
	this->ptr = ptr;
	this->i = i;
}

BPTreeNode::BPTreeNode(int n, bool leaf) {
	this->n = n;
	this->leaf = leaf;
}

NodeLocation BPTreeNode::minNode() {
	if (this->leaf) {
		return NodeLocation(this, 0);
	} else {
		return this->child[0]->minNode();
	}
}

NodeLocation BPTreeNode::maxNode() {
	if (this->leaf) {
		return NodeLocation(this, this->n-1);
	} else {
		return this->child[this->n]->maxNode();
	}
}

NodeLocation BPTreeNode::precursor(int keyIndex) {
	return this->child[keyIndex]->maxNode();
}

NodeLocation BPTreeNode::successor(int keyIndex) {
	return this->child[keyIndex+1]->minNode();
}

int BPTreeNode::simpleInsertLeft(int index, int key, int value, BPTreeNode* ptr) {
	for (int i=this->n-1; i >= index; i--) {
		this->keys[i+1] = this->keys[i];
		this->values[i+1] = this->values[i];
		this->child[i+2] = this->child[i+1];
	}
	this->child[index+1] = this->child[index];

	this->keys[index] = key;
	this->values[index] = value;
	this->child[index] = ptr;
	this->n += 1;
	return 0;
}

int BPTreeNode::simpleInsertRight(int index, int key, int value, BPTreeNode* ptr) {
	for (int i = this->n - 1; i >= index; i--) {
		this->keys[i + 1] = this->keys[i];
		this->values[i + 1] = this->values[i];
		this->child[i + 2] = this->child[i + 1];
	}
	this->keys[index] = key;
	this->values[index] = value;
	this->child[index + 1] = ptr;
	this->n += 1;
	return 0;
}


int BPTreeNode::leafRemove(int index) {
	for (int i=index+1; i < this->n; i++) {
		this->keys[i-1] = this->keys[i];
		this->values[i-1] = this->values[i];
	}
	this->n -= 1;
	return 0;
}

int BPTreeNode::simpleRemoveRight(int index) {
	if (this->leaf) {
		return this->leafRemove(index);
	} else {
		//delete(this->child[index + 1]);
		for (int i = index + 1; i < this->n; i++) {
			this->keys[i - 1] = this->keys[i];
			this->values[i - 1] = this->values[i];
			this->child[i] = this->child[i + 1];
		}
		this->n -= 1;
		return 0;
	}
}

int BPTreeNode::simpleRemoveLeft(int index) {
	if (this->leaf) {
		return this->leafRemove(index);
	}
	else {
		//delete(this->child[index]);
		for (int i = index + 1; i < this->n; i++) {
			this->keys[i - 1] = this->keys[i];
			this->values[i - 1] = this->values[i];
			this->child[i - 1] = this->child[i];
		}
		this->child[this->n - 1] = this->child[this->n];
		this->n -= 1;
		return 0;
	}
}

int BPTreeNode::mergeChild(int keyIndex) {
	BPTreeNode* y = this->child[keyIndex];
	BPTreeNode* z = this->child[keyIndex+1];
	
	y->keys[y->n] = this->keys[keyIndex];
	y->values[y->n] = this->values[keyIndex];
	y->n++;

	for (int i=0; i<MINKEYS; i++) {
		y->keys[y->n] = z->keys[i];
		y->values[y->n] = z->values[i];
		y->child[y->n] = z->child[i];
		y->n++;
	}
	y->child[y->n] = z->child[z->n];
	delete(z);
	this->simpleRemoveRight(keyIndex);
	return 0;
}

int find(int *list, int length, int key) {
    int bot = 0;
    int top = length - 1;
    int mid;
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
	if (this->n <= 0) {
		return 2;
	}
	int index = find(this->keys, this->n, key);
	if (this->keys[index] == key) {
		if (this->leaf) {
			this->leafRemove(index);
		} else {
			if (this->child[index]->n >= MIN) {
				NodeLocation pre = this->precursor(index);
				this->keys[index] = pre.ptr->keys[pre.i];
				this->values[index] = pre.ptr->values[pre.i];
				this->child[index]->remove(pre.ptr->keys[pre.i]);
			} else if (this->child[index+1]->n >= MIN) {
				NodeLocation suc = this->successor(index);
				this->keys[index] = suc.ptr->keys[suc.i];
				this->values[index] = suc.ptr->values[suc.i];
				this->child[index+1]->remove(suc.ptr->keys[suc.i]);
			} else {
				this->mergeChild(index);
				this->child[index]->remove(key);
			}
		}
	} else {
		if (this->leaf) {
			return 1;
		}

		if (key > this->keys[index]) {
			index += 1;
		}
		if (this->child[index]->n >= MIN) {
			return this->child[index]->remove(key);
		} else {
			if (index > 0 && this->child[index-1]->n >= MIN) {
				BPTreeNode* prev = this->child[index-1];
				this->child[index]->simpleInsertLeft(0,this->keys[index-1],
					this->values[index-1], prev->child[prev->n]);
				this->keys[index-1] = prev->keys[prev->n-1];
				this->values[index-1] = prev->values[prev->n-1];
				prev->simpleRemoveRight(prev->n-1);
			} else if (index < this->n && this->child[index+1]->n >= MIN) {
				BPTreeNode* next = this->child[index+1];
				this->child[index]->simpleInsertRight(this->child[index]->n,this->keys[index],
					this->values[index], next->child[0]);
				this->keys[index] = next->keys[0];
				this->values[index] = next->values[0];
				next->simpleRemoveLeft(0);
			} else {
				if (index >= this->n) {
					index -= 1;
				}
				this->mergeChild(index);
			}
			return this->child[index]->remove(key);
		}	
	}
	return 0;
}

int BPTreeNode::get(int key) {
	int index = -1;
	BPTreeNode* node = this->search(key, &index);
	if (!node) {
		//printf("warning: key %d doesn't exists.\n", key);
		return 0;
	} else {
		return node->values[index];
	}
	// printf("zkdebug: %d\n", (int)node);
}

int BPTreeNode::walk() {
	for (int i = 0; i < this->n; i++) {
		if (!this->leaf) {
			this->child[i]->walk();
		}
		printf("key: %d, value: %d.\n", this->keys[i], this->values[i]);
	}
	if (!this->leaf) {
		this->child[this->n]->walk();
	}
	return 0;
}

int BPTreeNode::size() {
	int sum = this->n;
	if (sum < 0) {
		printf("size error: %d", sum);
	}
	if (!this->leaf) {
		for (int i = 0; i <= this->n; i++) {
			sum += this->child[i]->size();
		}
	}
	return sum;
}

BPTreeNode* BPTreeNode::search(int key, int* index){
	int i = 0;
	while (i < this->n && key > this->keys[i]) {
		i += 1;
	}
	if (i < this->n && key == this->keys[i]) {
		*index = i;
		return this;
	} else if (this->leaf) {
		return NULL;
	} else {
		return this->child[i]->search(key, index);
	}
}


int BPTreeNode::insertNonFull(int key, int value) {
	int i = this->n - 1;
	if (this->leaf) {
		while (i >= 0 && key < this->keys[i]) {
			this->keys[i+1] = this->keys[i];
			this->values[i+1] = this->values[i];
			i -= 1;
		}
		this->keys[i+1] = key;
		this->values[i+1] = value;
		this->n += 1;
	} else {
		while (i >= 0 && key < this->keys[i]) {
			i -= 1;
		}
		i += 1;
		if (this->child[i]->n == MAX - 1) {
			this->splitChild(i);
			if (key > this->keys[i]) {
				i += 1;
			}
		}
		this->child[i]->insertNonFull(key, value);
	}
	return 0;
}

int BPTreeNode::splitChild(int index) {
	BPTreeNode* y = this->child[index];
	BPTreeNode* z = new BPTreeNode(MINKEYS, y->leaf);
	for (int i=0; i < MINKEYS; i++) {
		z->keys[i] = y->keys[i + MIN];
		z->values[i] = y->values[i + MIN];
	}
	if (!y->leaf) {
		for (int i=0; i < MIN; i++) {
			z->child[i] = y->child[i + MIN];
		}
	}
	y->n = MINKEYS;
	for (int i=this->n; i > index; i--) {
		this->child[i + 1] = this->child[i];
	}
	this->child[index + 1] = z;
	for (int i=this->n - 1; i >= index; i--) {
		this->keys[i + 1] = this->keys[i];
		this->values[i + 1] = this->values[i];
	}
	this->keys[index] = y->keys[MINKEYS];
	this->values[index] = y->values[MINKEYS];
	this->n += 1;
	return 0;
}


BPTree::BPTree() {
	BPTreeNode* x = new BPTreeNode(0, true);
	this->root = x;
}

int BPTree::insert(int key, int value) {
	BPTreeNode* root = this->root;
	int index = -1;
	BPTreeNode* node = this->root->search(key, &index);
	if (node) {
		node->values[index] = value;
	} else {
		if (root->n == MAX - 1) {
			BPTreeNode* s = new BPTreeNode(0, false);
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
		BPTreeNode* newRoot = this->root->child[0];
		delete(this->root);
		this->root = newRoot;
	}
	return v;
}
