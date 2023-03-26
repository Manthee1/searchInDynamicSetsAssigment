
#include "AVLTree.h"
#include <iostream>
using namespace std;

int AVLTree::getHeight(AVLNode *node) { return (node == NULL) ? 0 : node->height; }
int AVLTree::getBalance(AVLNode *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }

AVLNode *AVLTree::rotate(AVLNode *x, RotateDirection direction) {
	AVLNode *y;
	if (direction == LEFT) {  // left rotation
		y = x->right;
		x->right = y->left;
		if (y->left != NULL) y->left->parent = x;

		y->left = x;
	} else {  // right rotation
		y = x->left;
		x->left = y->right;
		if (y->right != NULL) y->right->parent = x;

		y->right = x;
	}

	// Update parent
	y->parent = x->parent;
	x->parent = y;

	// Parent clause
	if (y->parent == NULL)
		root = y;
	else if (y->parent->left == x)
		y->parent->left = y;
	else
		y->parent->right = y;

	// Update heights
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));

	// Return new root
	return y;
}

void AVLTree::balanceTree(AVLNode *node, int newKey) {
	while (node != NULL) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);

		// Update height
		node->height = 1 + max(leftHeight, rightHeight);

		// Check balance
		int balance = leftHeight - rightHeight;

		// If the node is unbalanced, then there are 4 cases
		//  1. if the new key is in the left subtree of the left child - right rotation
		//  2. if the new key is in the right subtree of the left child - left rotation followed by right rotation
		//  3. if the new key is in the right subtree of the right child - left rotation
		//  4. if the new key is in the left subtree of the right child - right rotation followed by left rotation
		if (balance > 1) {
			if (newKey < node->left->key)
				node = rotate(node, RIGHT);
			else {
				rotate(node->left, LEFT);
				node = rotate(node, RIGHT);
			}
		} else if (balance < -1) {
			if (newKey > node->right->key)
				node = rotate(node, LEFT);
			else {
				rotate(node->right, RIGHT);
				node = rotate(node, LEFT);
			}
		}

		node = node->parent;
	}
}

void AVLTree::balanceTree(AVLNode *node) {
	while (node != NULL) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);

		// Update height
		node->height = 1 + max(leftHeight, rightHeight);

		int balance = leftHeight - rightHeight;

		// If the node is unbalanced, then there are 4 cases
		//  1. if the node is left heavy and the left child is left heavy - right rotation
		//  2. if the node is left heavy and the left child is right heavy - left rotation followed by right rotation
		//  3. if the node is right heavy and the right child is right heavy - left rotation
		//  4. if the node is right heavy and the right child is left heavy - right rotation followed by left rotation
		if (balance > 1) {
			if (getBalance(node->left) >= 0)
				node = rotate(node, RIGHT);
			else {
				rotate(node->left, LEFT);
				node = rotate(node, RIGHT);
			}
		} else if (balance < -1) {
			if (getBalance(node->right) <= 0)
				node = rotate(node, LEFT);
			else {
				rotate(node->right, RIGHT);
				node = rotate(node, LEFT);
			}
		}

		node = node->parent;
	}
}

AVLNode *root;
AVLTree::AVLTree() {
	size = 0;
	root = NULL;
}
// Variadic constructor with std::initializer_list
AVLTree::AVLTree(std::initializer_list<int> keys, std::initializer_list<int> values) {
	size = 0;
	root = NULL;
}
// Constructor with array
AVLTree::AVLTree(int *keys, int *values, int size) {
	this->size = 0;
	root = NULL;
	for (int i = 0; i < size; i++)
		insert(keys[i], values[i]);
}

AVLTree::~AVLTree() {
	// Delete all nodes
	deleteTree(root);
}

void AVLTree::deleteTree(AVLNode *node) {
	if (node == NULL) return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}

void AVLTree::insert(int key, int value) { insertNode(new AVLNode(key, value), true); }

void AVLTree::insertNode(AVLNode *node, bool balance) {
	if (node == NULL) return;

	// If the tree is empty, make the node the root
	if (root == NULL) {
		root = node;
		size++;
		return;
	}

	// Find the appropriate parent for the node (make it a leaf)
	AVLNode *current = root;
	AVLNode *parent = NULL;
	while (current != NULL) {
		parent = current;
		// If the key is less than the current node's key, go to the left
		if (node->key < current->key) {
			current = current->left;
			continue;
		}
		// If the key already exists, increment the count and return
		if (node->key == current->key) {
			current->count++;
			current->value = node->value;
			delete node;
			return;
		}
		// Else, go to the right
		current = current->right;
	}

	// Insert the node depending on its key value
	if (node->key < parent->key)
		parent->left = node;
	else
		parent->right = node;

	// Set the parent
	node->parent = parent;

	// Update the size and count
	size++;
	node->count = 1;

	// Balance the tree
	if (balance)
		balanceTree(node->parent);
}

AVLNode *AVLTree::searchKey(int key) {
	AVLNode *current = root;
	// Search for the node with the given key
	while (current != NULL) {
		if (current->key == key) return current;
		// If the key is less than the current node's key, search the left subtree
		current = (key < current->key) ? current->left : current->right;
	}
	return NULL;
}

void AVLTree::deleteKey(int key) {
	AVLNode *node = searchKey(key);
	deleteNode(node);
}

void AVLTree::deleteNode(AVLNode *node) {
	if (node == NULL) return;

	// If the node only has one direct child
	if ((node->right == NULL) ^ (node->left == NULL)) {
		// Set the child as the node's parent's left or right child
		AVLNode *child = (node->right == NULL) ? node->left : node->right;
		// If the node is the root, set the child as the new root
		// Otherwise, set the child as the node's parent's left or right child
		if (node->parent == NULL) {
			root = child;
			child->parent = NULL;
		} else if (node->parent->left == node) {
			node->parent->left = child;
			child->parent = node->parent;
		} else {
			node->parent->right = child;
			child->parent = node->parent;
		}
		size--;
		balanceTree(node->parent);
		delete node;
		return;
	}

	// If the node has both children
	// Here we don't delete the node, but we find the largest node in the left subtree and copy its key to the node to be "deleted"
	// After that, we delete the very node we copied the key from, because it has at most one child
	if (node->right != NULL && node->left != NULL) {
		// Find the largest node in the left subtree
		AVLNode *removeNode = node->left;
		while (removeNode->right != NULL)
			removeNode = removeNode->right;

		// Copy the largest subtree's node's key to the node to be deleted
		node->key = removeNode->key;
		node->value = removeNode->value;

		// Make a pointer to the node to be deleted's parent
		AVLNode *tempLeafNode;
		tempLeafNode = removeNode->parent;

		// Delete the largest node
		deleteNode(removeNode);

		// Use the tempLeafNode pointer to balance the tree
		balanceTree(tempLeafNode);

		return;
	}

	// === If the node is a leaf ===
	size--;

	// if the node is the root just re2move it
	if (node->parent == NULL) {
		root = NULL;
		delete node;
		return;
	}

	// Otherwise find which child of the parent is the node and set it to null
	if (node->parent->left == node)
		node->parent->left = NULL;
	else
		node->parent->right = NULL;

	balanceTree(node->parent);
	delete node;
}
