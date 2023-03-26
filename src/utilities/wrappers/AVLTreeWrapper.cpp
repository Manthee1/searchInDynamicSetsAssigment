#include <iostream>
#include "AVLTreeWrapper.h"

AVLTree* tree;

AVLTreeWrapper::AVLTreeWrapper() {
	this->name = "AVL Tree";
}

void AVLTreeWrapper::init(int) {
	tree = new AVLTree();
};
void AVLTreeWrapper::insert(int key) {
	tree->insert(key, key);
}
int AVLTreeWrapper::search(int key) {
	AVLNode* node = tree->searchKey(key);
	return node == NULL ? -1 : node->key;
}
void AVLTreeWrapper::returnlessSearch(int key) {
	tree->searchKey(key);
}
void AVLTreeWrapper::remove(int key) {
	tree->deleteKey(key);
}
bool AVLTreeWrapper::isValid() {
	return isAVLTreeBalanced(tree, tree->root);
}
int AVLTreeWrapper::getSize() {
	return tree->size;
}
long long AVLTreeWrapper::calculateSpaceComplexity() {
	// Simple
	return sizeof(AVLNode) * tree->size;
}
void AVLTreeWrapper::destroy() {
	delete tree;
}
AVLTreeWrapper::~AVLTreeWrapper() {
	delete tree;
}

// Go through each node and check if the balance is correct
static bool isAVLTreeBalanced(AVLTree* tree, AVLNode* node) {
	if (node == NULL) return true;
	if (abs(tree->getBalance(node)) > 1) return false;
	return (node->left == NULL || isAVLTreeBalanced(tree, node->left)) &&
		   (node->right == NULL || isAVLTreeBalanced(tree, node->right));
}