#include "RedBlackTree.h"

RedBlackTree::RedBlackTree() {
	size = 0;
	this->height = 0;
	root = NULL;
}
RedBlackTree::RedBlackTree(std::initializer_list<int> list) {
	size = 0;
	this->height = 0;
	root = NULL;
	// Insert all keys
	for (int key : list)
		insertKey(key);
}

RedBlackTree::RedBlackTree(int *list, int size) {
	this->size = 0;
	this->height = 0;
	root = NULL;
	// Insert all keys
	for (int i = 0; i < size; i++)
		insertKey(list[i]);
}

RedBlackTree::~RedBlackTree() {
	deleteTree(root);
}

void RedBlackTree::deleteTree(RedBlackNode *node) {
	if (node == NULL) return;
	deleteTree(node->left);
	deleteTree(node->right);
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

void RedBlackTree::insertKey(int key) {
	insertNode(new RedBlackNode(key));
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
	if (node == NULL) {
		// The key is not in the tree, so there's nothing to delete
		return;
	}
	// Save the original color of the node to be deleted
	bool originalColor = node->color;
	// Declare two pointers to nodes
	RedBlackNode *x, *y;
	if (node->left == NULL) {
		// If the node has no left child, replace it with its right child
		x = node->right;
		transplant(node, node->right);
	} else if (node->right == NULL) {
		// If the node has no right child, replace it with its left child
		x = node->left;
		transplant(node, node->left);
	} else {
		// If the node has both left and right children, replace it with its successor
		y = node->right;
		while (y->left != NULL) {
			y = y->left;
		}
		originalColor = y->color;
		// Replace the successor with its right child
		x = y->right;
		// If the successor is the right child of the node to be deleted, replace the successor with its right child
		if (y->parent != node) {
			transplant(y, y->right);
			y->right = node->right;
			y->right->parent = y;
		}
		// Replace the node to be deleted with the successor
		transplant(node, y);
		y->left = node->left;
		y->left->parent = y;
		y->color = node->color;
	}
	// If the original color of the deleted node was black, and the replacement node x is not null,
	// rebalance the tree starting from x
	if (originalColor == black && x != NULL) {
		fixDeleteFrom(x);
	}
	// Delete the node
	delete node;
}

void RedBlackTree::fixDeleteFrom(RedBlackNode *node) {
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
