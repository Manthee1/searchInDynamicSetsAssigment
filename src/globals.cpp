#include "globals.h"

AVLNode *createNode(int key) {
	AVLNode *newNode = new AVLNode;
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	newNode->height = 1;
	return newNode;
}
