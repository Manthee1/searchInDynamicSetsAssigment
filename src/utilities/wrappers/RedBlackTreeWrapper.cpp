#include "RedBlackTreeWrapper.h"

static RedBlackTree* tree;

RedBlackTreeWrapper::RedBlackTreeWrapper() {
	this->name = "RedBlackTree";
}
void RedBlackTreeWrapper::init(int) {
	tree = new RedBlackTree();
}
void RedBlackTreeWrapper::insert(int key) {
	tree->insert(key, key);
}
int RedBlackTreeWrapper::search(int key) {
	RedBlackNode* node = tree->searchKey(key);
	return node == NULL ? -1 : node->key;
}
void RedBlackTreeWrapper::returnlessSearch(int key) {
	tree->searchKey(key);
}
void RedBlackTreeWrapper::remove(int key) {
	tree->deleteKey(key);
}
bool RedBlackTreeWrapper::isValid() {
	return isRedBlackTreeValid(tree);
}
int* RedBlackTreeWrapper::getAllKeys(int& length) {
	return getAllRedBlackTreeKeys(tree, tree->root, length);
}
int RedBlackTreeWrapper::calculateSpaceComplexity() {
	return sizeof(RedBlackNode) * tree->size;
}
void RedBlackTreeWrapper::destroy() {
	delete tree;
}
RedBlackTreeWrapper::~RedBlackTreeWrapper() {
	delete tree;
}

static int* getAllRedBlackTreeKeys(RedBlackTree* tree, RedBlackNode* node, int& length) {
	if (node == NULL) return NULL;
	int* leftKeys = getAllRedBlackTreeKeys(tree, node->left, length);
	int* rightKeys = getAllRedBlackTreeKeys(tree, node->right, length);
	int* keys = new int[length + 1];
	for (int i = 0; i < length; i++) {
		keys[i] = leftKeys[i];
	}
	keys[length] = node->key;
	length++;
	for (int i = 0; i < length; i++) {
		keys[length + i] = rightKeys[i];
	}
	length += length;
	return keys;
}

/**
 *@brief Checks if a red-black tree is valid.
 *
 * @param tree
 * @param node
 * @param blackHeight
 * @param numBlackNodes
 * @return true
 * @return false
 */
static bool isRedBlackTreeValidHelper(RedBlackTree* tree, RedBlackNode* node, int& blackHeight, int& numBlackNodes) {
	// If we've reached the end of a path (i.e., a null node), check the black height of the path and return true.
	if (node == NULL) {
		// If this is the first time we're encountering a null node, save the black height.
		if (blackHeight == -1) blackHeight = numBlackNodes;

		// Otherwise, if the current path's black height doesn't match the saved black height, the tree is invalid.
		else if (blackHeight != numBlackNodes)
			return false;

		// Return true since we've reached the end of a path.
		return true;
	}

	// Check that a red node doesn't have a red child.
	if (node->color == red) {
		if (node->left != NULL && node->left->color == red) return false;
		if (node->right != NULL && node->right->color == red) return false;
	}

	// If we've encountered a black node, increment the number of black nodes on this path.
	if (node->color == black) numBlackNodes++;

	// Recursively validate the left and right subtrees, and return false if either is invalid.
	bool leftValid = isRedBlackTreeValidHelper(tree, node->left, blackHeight, numBlackNodes);
	if (!leftValid) return false;

	bool rightValid = isRedBlackTreeValidHelper(tree, node->right, blackHeight, numBlackNodes);
	if (!rightValid) return false;

	// If we've encountered a black node, decrement the number of black nodes on this path.
	if (node->color == black) numBlackNodes--;

	// If we've made it to the end of a path and haven't returned false, the tree is valid.
	return true;
}

bool isRedBlackTreeValid(RedBlackTree* tree) {
	RedBlackNode* root = tree->root;
	int blackHeight = -1;
	int numBlackNodes = 0;
	return isRedBlackTreeValidHelper(tree, root, blackHeight, numBlackNodes);
}