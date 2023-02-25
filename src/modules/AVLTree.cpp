
#include "AVLTree.h"
#include <iostream>
using namespace std;

Node *AVLTree::leftRotate(Node *x) {
	if (x->right == NULL) return x;
	Node *y = x->right;

	if (y->left != NULL) {
		Node *tempNode = y->left;
		x->right = tempNode;
		tempNode->parent = x;
	} else
		x->right = NULL;

	// If the parent of x is null, then x is the root. So set y as the root
	if (x->parent == NULL) root = y;
	// Otherwise, set y as the left or right child of x's parent
	else if (x->parent->left == x)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->parent = x->parent;
	x->parent = y;
	y->left = x;

	// Update heights
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));

	// Return new root
	return y;
}

Node *AVLTree::rightRotate(Node *y) {
	if (y->left == NULL) return y;
	Node *x = y->left;

	if (x->right != NULL) {
		Node *tempNode = x->right;
		y->left = tempNode;
		tempNode->parent = y;
	} else
		y->left = NULL;

	// If the parent of y is null, then y is the root. So set x as the root
	if (y->parent == NULL) root = x;
	// Otherwise, set x as the left or right child of y's parent
	else if (y->parent->right == y)
		y->parent->right = x;
	else
		y->parent->left = x;

	x->parent = y->parent;
	y->parent = x;
	x->right = y;

	// Update heights
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));

	return x;
}

void AVLTree::balanceTree(Node *node, int newKey) {
	if (node == NULL) return;

	// Update height
	node->height = 1 + max(getHeight(node->left), getHeight(node->right));

	int balance = getBalance(node);
	if (balance > 1) {
		if (newKey < node->left->key)
			node = rightRotate(node);
		else {
			node->left = leftRotate(node->left);
			node = rightRotate(node);
		}
	} else if (balance < -1) {
		if (newKey > node->right->key)
			node = leftRotate(node);
		else {
			node->right = rightRotate(node->right);
			node = leftRotate(node);
		}
	}

	balanceTree(node->parent, newKey);
}

void AVLTree::balanceTree(Node *node) {
	if (node == NULL) return;

	// Update height
	node->height = 1 + max(getHeight(node->left), getHeight(node->right));

	int balance = getBalance(node);
	if (balance > 1) {
		if (getBalance(node->left) >= 0)
			node = rightRotate(node);
		else {
			node->left = leftRotate(node->left);
			node = rightRotate(node);
		}
	} else if (balance < -1) {
		if (getBalance(node->right) <= 0)
			node = leftRotate(node);
		else {
			node->right = rightRotate(node->right);
			node = leftRotate(node);
		}
	}

	balanceTree(node->parent);
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

int AVLTree::getHeight(Node *node) { return (node == NULL) ? 0 : node->height; }
int AVLTree::getBalance(Node *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }

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
		if (node->key < current->key) {
			// If the key is less than the current node's key, go to the left
			current = current->left;
			continue;
		} else if (node->key == current->key) {
			// If the key already exists, delete the node and return
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
		if (node->parent == NULL)
			root = child;
		else if (node->parent->left == node) {
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
