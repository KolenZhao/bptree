#include "bptree_test.h"
#include "bptree.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define TEST_MAX 10000000


int leakTest() {
	BPTree* T = new BPTree();

	int* testList = (int*)malloc(sizeof(int) * TEST_MAX);
	srand((unsigned int)time(NULL));

	for (int c = 0; c < 65536; c++) {
		for (int i = 0; i < TEST_MAX; i++) {
			int r = (rand() << 16) | rand();
			testList[i] = r;
		}
		for (int i = 0; i < TEST_MAX; i++) {
			T->insert(testList[i], testList[i] / 2);
		}
		int size = T->root->size();
		printf("size: %d. ## ", size);
		int re = 0;
		int empty = 0;
		for (int i = 0; i < TEST_MAX; i++) {
			int rv = T->remove(testList[i]);
			if (rv == 1) {
				re++;
				//printf("Info: key does not exist. key: %d\n", testList[i]);
			}
			else if (rv == 2) {
				empty++;
				//printf("Warning: root is empty. key: %d\n", testList[i]);
			}
			else if (rv) {
				printf("Error: remove failed. key: %d\n", testList[i]);
			}
		}
		printf("empty time: %d, redundant time: %d, sum: %d. ## ", empty, re, size + empty + re);
		printf("size: %d\n", T->root->size());
	}
	return 0;
}


int funcTest() {
	BPTree* T = new BPTree();

	//int testList[TEST_MAX];

	int* testList = (int*)malloc(sizeof(int) * TEST_MAX);
	srand((unsigned int)time(NULL));
	for (int i = 0; i < TEST_MAX; i++) {
		int r = (rand() << 16) | rand();
		//int r = rand();
		testList[i] = r;
	}
	//int testList[TEST_MAX] = {5890, 777, 27365, 25899, 32696, 30499, 21020, 31291, 8368, 13063, 1099, 27477, 2728, 16520, 28624, 2545, 13198, 16108, 29602, 17086, 30879, 27786, 4752, 5084, 3024, 28312, 27593, 13132, 20589, 19262, 7329, 31141, 27392, 18926, 28562, 25864, 2164, 11768, 31873, 18517, 8314, 32601, 7747, 31391, 14124, 21972, 24928, 6878, 25072, 23109, 20503, 15332, 5024, 29233, 16290, 3219, 29672, 18849, 699, 20556, 17903, 12081, 9566, 15274, 8761, 30308, 5585, 2417, 1996, 21562, 24290, 23489, 2193, 27141, 22384, 15760, 5374, 29304, 27436, 22615, 10994, 17665, 24737, 12810, 27616, 9049, 16498, 1870, 24112, 1369, 16867, 5607, 24034, 22287, 24731, 27503, 117, 23825, 32681, 3150};

	for (int i = 0; i < TEST_MAX; i++) {
		T->insert(testList[i], testList[i] / 2);
		//printf("size: %d\n", T->root->trueSize());
	}
	int size = T->root->size();
	printf("size: %d\n", size);

	for (int i = 0; i < TEST_MAX; i++) {
		int v = T->root->get(testList[i]);
		if (v != testList[i] / 2) {
			printf("ERROR: Value wrong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!########################\n");
		}
	}
	//T->root->walk();

	int re = 0;
	int empty = 0;
	for (int i = 0; i < TEST_MAX; i++) {
		if (i == 81) {
			re = re;
		}

		//if (i == TEST_MAX / 2) {
		//	for (int j = i; j < TEST_MAX; j++) {
		//		int v = T->root->get(testList[j]);
		//		if (v != testList[j] / 2) {
		//			printf("ERROR: 2 Value wrong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!########################\n");
		//		}
		//	}
		//}

		int rv = T->remove(testList[i]);
		if (rv == 1) {
			re++;
			//printf("Info: key does not exist. key: %d\n", testList[i]);
		}
		else if (rv == 2) {
			empty++;
			//printf("Warning: root is empty. key: %d\n", testList[i]);
		}
		else if (rv) {
			printf("Error: remove failed. key: %d\n", testList[i]);
		}
		else {

		}
		//int size = T->root->size();
		//int trueSize = T->root->trueSize();
		//if (trueSize != size) {
		//	printf("Erorr: SIZE WRONG. size != trueSize. size: %d, trueSize: %d", size, trueSize);
		//}
		//printf("size: %d\n", trueSize);
		//for (int ii = 0; ii < TEST_MAX; ii++) {
		//	printf("%d, ", testList[ii]);
		//}
	}

	printf("empty time: %d, redundant time: %d, sum: %d\n", empty, re, size + empty + re);
	printf("size: %d\n", T->root->size());

	return 0;
}

int main() {
	//leakTest();

	funcTest();

	//printf("%d\n", sizeof(BTreeNode));
	system("pause");
	return 0;
}


