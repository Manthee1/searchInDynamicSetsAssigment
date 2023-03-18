#include "RedBlackTree.h"

RedBlackTree::RedBlackTree() {
	size = 0;
	this->height = 0;
	root = NULL;
}
RedBlackTree::RedBlackTree(std::initializer_list<int> keys, std::initializer_list<int> values) {
	size = 0;
	this->height = 0;
	root = NULL;
	// Insert all keys
	// for (int key : keys)
	// 	insertKey(key);
}

RedBlackTree::RedBlackTree(int *keys, int *values, int size) {
	this->size = 0;
	this->height = 0;
	root = NULL;
	// Insert all keys
	for (int i = 0; i < size; i++)
		insert(keys[i], values[i]);
}

RedBlackTree::~RedBlackTree() {
	deleteTree(root);
}

void RedBlackTree::deleteTree(RedBlackNode *node) {
	if (node == NULL) return;
	deleteTree(node->left);
	deleteTree(node->right);
	size = 0;
	delete node;
}

RedBlackNode *RedBlackTree::rotate(RedBlackNode *x, RotateDirection direction) {
	RedBlackNode *y;
	// Setup for left rotation
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

	// Return new root
	return y;
}

void RedBlackTree::transplant(RedBlackNode *node1, RedBlackNode *node2) {
	if (node2 != NULL)
		node2->parent = node1->parent;

	// If the parent of node1 is null, set the root to node2
	if (node1->parent == NULL) {
		root = node2;
		return;
	}
	// If node1 is the left child of its parent, set the left child of the parent to node2
	// Otherwise, set the right child of the parent to node2
	(node1 == node1->parent->left) ? node1->parent->left = node2 : node1->parent->right = node2;
}

RedBlackNode *RedBlackTree::successor(RedBlackNode *node) {
	if (node->right != nullptr) {
		node = node->right;
		while (node->left != nullptr)
			node = node->left;

		return node;
	}
	RedBlackNode *parent = node->parent;
	while (parent != nullptr && node == parent->right) {
		node = parent;
		parent = parent->parent;
	}
	return parent;
}

void RedBlackTree::fixInsertFrom(RedBlackNode *node) {
	RedBlackNode *parent;
	// Continue looping until parent node is black
	while (node->parent != NULL && node->parent->color == red) {
		parent = node->parent;
		// Determine which side of the grandparent node the parent is on
		bool parentIsLeft = (parent == parent->parent->left);
		RedBlackNode *uncle = parentIsLeft ? parent->parent->right : parent->parent->left;
		// Case 1: Uncle node is red
		if (uncle != NULL && uncle->color == red) {
			parent->color = black;
			uncle->color = black;
			parent->parent->color = red;
			node = parent->parent;
		}
		// Case 2: Uncle node is black
		else {
			// Determine whether node is on the same side as the parent or opposite side
			bool nodeIsLeft = (node == (parentIsLeft ? parent->left : parent->right));
			// If node is on the opposite side of the parent, rotate node up to be on the same side
			if (!nodeIsLeft) {
				node = parent;
				rotate(node, parentIsLeft ? LEFT : RIGHT);
				parent = node->parent;
			}
			// Recolor parent and grandparent, and rotate grandparent
			parent->color = black;
			parent->parent->color = red;
			rotate(parent->parent, parentIsLeft ? RIGHT : LEFT);
		}
		if (node == root) break;
	}
	root->color = black;
}

void RedBlackTree::insert(int key, int value) {
	insertNode(new RedBlackNode(key, value));
}

void RedBlackTree::insertNode(RedBlackNode *node) {
	if (root == NULL) {
		root = node;
		root->color = black;
		return;
	}

	// Initialize current and parent
	RedBlackNode *current = this->root;
	RedBlackNode *parent = NULL;  // Future parent of the node

	// Find the parent of the node
	while (current != NULL) {
		parent = current;
		if (node->key == current->key) {
			current->count++;
			delete node;
			return;
		}
		current = (node->key < current->key) ? current->left : current->right;
	}

	// Set the parent of the node
	node->parent = parent;
	// Set the node as the left or right child of the parent
	(node->key < parent->key) ? parent->left = node
							  : parent->right = node;

	// If the parent is the root, the tree is balanced
	if (node->parent == NULL) {
		node->color = black;
		return;
	}

	// If the parent is black, or the grandparent is NULL, the tree is balanced
	if (node->parent->color == black || node->parent->parent == NULL) return;

	size++;

	// Balance the tree
	fixInsertFrom(node);
}

RedBlackNode *RedBlackTree::searchKey(int key) {
	RedBlackNode *current = root;
	// Search for the node with the given key
	while (current != NULL) {
		if (key == current->key) return current;
		// If the key is less than the current node's key, search the left subtree
		current = (key < current->key) ? current->left : current->right;
	}
	return NULL;
}

void RedBlackTree::deleteKey(int key) {
	deleteNode(searchKey(key));
}

void RedBlackTree::deleteNode(RedBlackNode *node) {
	if (node == nullptr) return;

	// Determine the node to delete (y) and its replacement (x)
	RedBlackNode *y = (node->left == nullptr || node->right == nullptr) ? node : successor(node);
	RedBlackNode *x = (y->left != nullptr) ? y->left : y->right;

	// If x is null, create a dummy NIL node
	if (x == nullptr) x = new RedBlackNode(-1, -1, black);

	// Set the parent of x to the parent of y
	x->parent = y->parent;

	// Replace y with x in the tree
	if (y->parent == nullptr)
		root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	// If y and node are different nodes, copy the values from y to node
	if (y != node) {
		node->key = y->key;
		node->value = y->value;
		node->count = y->count;
	}

	// If y was black, fix the tree starting from x
	if (y->color == black) fixDeleteFrom(x);

	// If x is a dummy NIL node, delete it
	if (x->key == -1 && x->value == -1) {
		if (x->parent == nullptr)  // If x is the root, set the root to null
			root = nullptr;
		else if (x == x->parent->left)	// If x is a left child, set the left child to null
			x->parent->left = nullptr;
		else  // If x is a right child, set the right child to null
			x->parent->right = nullptr;

		delete x;
	}

	// Delete y
	delete y;
}

void RedBlackTree::fixDeleteFrom(RedBlackNode *node) {
	if (node == NULL) return;

	// Initialize variables
	RedBlackNode *sibling;
	RedBlackNodeColor siblingLeftChildColor;
	RedBlackNodeColor siblingRightChildColor;

	while (node != root && node->color == black) {
		// Determine if node is a left or right child of its parent
		bool isLeftChild = (node == node->parent->left);

		// Get sibling and its child colors based on whether node is a left or right child
		sibling = (isLeftChild) ? node->parent->right : node->parent->left;
		siblingLeftChildColor = (sibling && sibling->left) ? sibling->left->color : black;
		siblingRightChildColor = (sibling && sibling->right) ? sibling->right->color : black;

		// Case 1: Sibling is red
		// Rotate the parent of node and make the sibling black
		if (sibling && sibling->color == red) {
			sibling->color = black;
			node->parent->color = red;
			rotate(node->parent, (isLeftChild) ? LEFT : RIGHT);
			sibling = (isLeftChild) ? node->parent->right : node->parent->left;
			siblingLeftChildColor = (sibling && sibling->left) ? sibling->left->color : black;
			siblingRightChildColor = (sibling && sibling->right) ? sibling->right->color : black;
		}

		// Case 2: Sibling and its children are black
		// Make the sibling red and move up the tree
		if ((!sibling || siblingLeftChildColor == black) && (!sibling || siblingRightChildColor == black)) {
			if (sibling) sibling->color = red;
			node = node->parent;
		} else {
			// Case 3: Sibling's outer child is black
			// Rotate the sibling and make the sibling's inner child black
			if ((isLeftChild && siblingRightChildColor == black) || (!isLeftChild && siblingLeftChildColor == black)) {
				if (sibling) sibling->color = red;
				if (isLeftChild) {
					if (sibling->right) sibling->right->color = black;
					rotate(sibling, RIGHT);
					sibling = node->parent->right;
				} else {
					if (sibling->left) sibling->left->color = black;
					rotate(sibling, LEFT);
					sibling = node->parent->left;
				}
				siblingLeftChildColor = (sibling && sibling->left) ? sibling->left->color : black;
				siblingRightChildColor = (sibling && sibling->right) ? sibling->right->color : black;
			}

			// Case 4: Sibling's outer child is red
			// Rotate the parent of node and make the sibling's outer child black
			if (sibling) sibling->color = node->parent->color;
			node->parent->color = black;
			if (isLeftChild) {
				if (sibling && sibling->right) sibling->right->color = black;
				rotate(node->parent, LEFT);
			} else {
				if (sibling && sibling->left) sibling->left->color = black;
				rotate(node->parent, RIGHT);
			}
			node = root;
		}
	}
	if (node) node->color = black;
}
