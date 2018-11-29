#define MIN (64)
#define MAX (MIN * 2)
#define MINKEYS (MIN - 1)

struct BPTreeNode;
struct LeafNode;

struct NodeLocation {
    BPTreeNode* ptr;
    int i;

    NodeLocation(BPTreeNode* ptr, int i);
};

struct BPTreeNode {
    int n;  // number of keys
    bool leaf;
    int keys[MAX-1];

    int insertNonFull(int key, int values);
    int remove(int key);
	LeafNode* search(int key, int* index);

	int walk();
	int size();
	int trueSize();
	int get(int key);
	LeafNode* first();
	NodeLocation getLocation(int key);
	
    int splitChild(int index);
    int mergeChild(int keyIndex);
    NodeLocation minNode();
    NodeLocation maxNode();
    NodeLocation precursor(int keyIndex);
    NodeLocation successor(int keyIndex);
};

struct InterNode:BPTreeNode {
	BPTreeNode* child[MAX];

	InterNode(int n);
	int simpleInsertRight(int index, int key, BPTreeNode* ptr);
	int simpleInsertLeft(int index, int key, BPTreeNode* ptr);
	int simpleRemoveRight(int index);
	int simpleRemoveLeft(int index);
};

struct LeafNode:BPTreeNode {
	int values[MAX-1];
	LeafNode* next;

	LeafNode(int n);
	int simpleInsert(int index, int key, int value);
	int leafRemove(int index);
};

struct BPTree {
    BPTreeNode* root;

    BPTree();
    int insert(int key, int value);
	int remove(int key);
};

BPTreeNode* newNode(int n, bool leaf);

int find(int *list, int length, int key);
