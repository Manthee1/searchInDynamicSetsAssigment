
#include "AVLTree.h"
#include <iostream>
using namespace std;

int AVLTree::getHeight(Node *node) { return (node == NULL) ? 0 : node->height; }
int AVLTree::getBalance(Node *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }

Node *AVLTree::rotate(Node *x, RotateDirection direction) {
	Node *y;
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

void AVLTree::balanceTree(Node *node, int newKey) {
	while (node != NULL) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);

		// Update height
		node->height = 1 + max(leftHeight, rightHeight);

		// Check balance
		int balance = leftHeight - rightHeight;
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

void AVLTree::balanceTree(Node *node) {
	while (node != NULL) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);

		// Update height
		node->height = 1 + max(leftHeight, rightHeight);

		int balance = leftHeight - rightHeight;
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

Node *root;
AVLTree::AVLTree() {
	size = 0;
	root = NULL;
}
// Variadic constructor with std::initializer_list
AVLTree::AVLTree(std::initializer_list<int> list) {
	size = 0;
	root = NULL;
	for (int key : list)
		insertKey(key);
}
// Constructor with array
AVLTree::AVLTree(int *list, int size) {
	this->size = 0;
	root = NULL;
	for (int i = 0; i < size; i++)
		insertKey(list[i]);
}

AVLTree::~AVLTree() {
	// Delete all nodes
	deleteTree(root);
}

void AVLTree::deleteTree(Node *node) {
	if (node == NULL) return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}

void AVLTree::insertKey(int key) { insertNode(createNode(key), true); }

void AVLTree::insertNode(Node *node, bool balance) {
	if (node == NULL) return;

	// If the tree is empty, make the node the root
	if (root == NULL) {
		root = node;
		size++;
		return;
	}

	// Find the appropriate parent for the node (make it a leaf)
	Node *current = root;
	Node *parent = NULL;
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
		balanceTree(node->parent, node->key);
}

Node *AVLTree::searchKey(int key) {
	Node *current = root;
	while (current != NULL) {
		if (current->key == key) return current;
		current = (key < current->key) ? current->left : current->right;
	}
	return NULL;
}

void AVLTree::deleteKey(int key) {
	Node *node = searchKey(key);
	deleteNode(node);
}

void AVLTree::deleteNode(Node *node) {
	if (node == NULL) return;

	// If the node only has one direct child
	if ((node->right == NULL) ^ (node->left == NULL)) {
		// Set the child as the node's parent's left or right child
		Node *child = (node->right == NULL) ? node->left : node->right;
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
	// After that, we delete the very node we copied the key from, because it has at most one child (easy to delete)
	if (node->right != NULL && node->left != NULL) {
		// Find the largest node in the left subtree
		Node *removeNode = node->left;
		while (removeNode->right != NULL)
			removeNode = removeNode->right;

		// Copy the largest subtree's node's key to the node to be deleted
		node->key = removeNode->key;

		// Make a pointer to the node to be deleted's parent
		Node *tempLeafNode;
		tempLeafNode = removeNode->parent;

		// Delete the largest node
		deleteNode(removeNode);

		// Use the tempLeafNode pointer to balance the tree
		balanceTree(tempLeafNode);

		return;
	}

	// === If the node is a leaf ===
	// if the node is the root just remove it
	if (node->parent == NULL) {
		root = NULL;
		size--;
		delete node;
		return;
	}

	// Otherwise find which child of the parent is the node and set it to null
	if (node->parent->left == node)
		node->parent->left = NULL;
	else
		node->parent->right = NULL;

	size--;
	balanceTree(node->parent);
	delete node;
}
