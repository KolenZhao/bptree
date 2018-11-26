
#define MIN (64)
#define MAX (MIN * 2)
#define MINKEYS (MIN - 1)

struct BPTreeNode;

struct NodeLocation {
    BPTreeNode* ptr;
    int i;

    NodeLocation(BPTreeNode* ptr, int i);
};

struct BPTreeNode {
    int n;  // number of keys
    bool leaf;
    BPTreeNode* child[MAX];
    int keys[MAX - 1];
    int values[MAX - 1];

    BPTreeNode(int n, bool leaf);

    int insertNonFull(int key, int values);
    int remove(int key);
    BPTreeNode* search(int key, int* index);
	int walk();
	int size();

    int get(int key);
    int splitChild(int index);
    int mergeChild(int keyIndex);

    NodeLocation minNode();
    NodeLocation maxNode();
    NodeLocation precursor(int keyIndex);
    NodeLocation successor(int keyIndex);
    int leafRemove(int index);
    int simpleRemoveRight(int index);
    int simpleRemoveLeft(int index);
    int simpleInsertLeft(int index, int key, int value, BPTreeNode* ptr);
	int simpleInsertRight(int index, int key, int value, BPTreeNode* ptr);
    //int inNodeFind(int key);
};

struct BPTree {
    BPTreeNode* root;

    BPTree();

    int insert(int key, int value);
	int remove(int key);
};

int find(int *list, int length, int key);