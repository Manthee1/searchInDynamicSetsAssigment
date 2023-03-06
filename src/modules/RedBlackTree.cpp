#include "RedBlackTree.h"

RedBlackTree::RedBlackTree() {
	size = 0;
	root = NULL;
}
RedBlackTree::RedBlackTree(std::initializer_list<int> list) {
	size = 0;
	root = NULL;
	for (int key : list)
		insertKey(key);
}

RedBlackTree::RedBlackTree(int *list, int size) {
	this->size = 0;
	root = NULL;
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
	while (node->parent != NULL && node->parent->color == red) {
		parent = node->parent;
		// If node has an uncle and the uncle is red
		if (parent == parent->parent->left) {
			RedBlackNode *uncle = parent->parent->right;
			if (uncle != NULL && uncle->color == red) {
				parent->color = black;
				uncle->color = black;
				parent->parent->color = red;
				node = parent->parent;
			} else {
				if (node == parent->right) {
					node = parent;
					rotate(node, LEFT);
					parent = node->parent;
				}
				parent->color = black;
				parent->parent->color = red;
				rotate(parent->parent, RIGHT);
			}
		} else {
			RedBlackNode *uncle = parent->parent->left;
			if (uncle != NULL && uncle->color == red) {
				parent->color = black;
				uncle->color = black;
				parent->parent->color = red;
				node = parent->parent;
			} else {
				if (node == parent->left) {
					node = parent;
					rotate(node, RIGHT);
					parent = node->parent;
				}
				parent->color = black;
				parent->parent->color = red;
				rotate(parent->parent, LEFT);
			}
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
	while (current != NULL) {
		if (key == current->key) return current;
		current = (key < current->key) ? current->left : current->right;
	}
	return NULL;
}

void RedBlackTree::deleteKey(int key) {
	deleteNode(searchKey(key));
}

void RedBlackTree::deleteNode(RedBlackNode *node) {
	if (node == NULL) return;
	RedBlackNode *x, *y;
	int key = node->key;
	while (node != NULL && node->key == key) {
		y = node;
		bool originalColor = y->color;
		if (node->left == NULL) {
			x = node->right;
			transplant(node, node->right);
		} else if (node->right == NULL) {
			x = node->left;
			transplant(node, node->left);
		} else {
			y = node->right;
			while (y->left != NULL) y = y->left;
			originalColor = y->color;
			x = y->right;
			if (y->parent != node) {
				transplant(y, y->right);
				y->right = node->right;
				y->right->parent = y;
			}
			transplant(node, y);
			y->left = node->left;
			y->left->parent = y;
			y->color = node->color;
		}
		if (originalColor == black && x != NULL) fixDeleteFrom(x);
		node = searchKey(key);
	}
}

void RedBlackTree::fixDeleteFrom(RedBlackNode *node) {
	RedBlackNode *sibling;
	RedBlackNodeColor siblingLeftChildColor;
	RedBlackNodeColor siblingRightChildColor;
	while (node != root && node->color == black) {
		if (node == node->parent->left) {
			sibling = node->parent->right;
			siblingLeftChildColor = sibling->left != NULL ? sibling->left->color : black;
			siblingRightChildColor = sibling->right != NULL ? sibling->right->color : black;
			if (sibling->color == red) {
				sibling->color = black;
				node->parent->color = red;
				rotate(node->parent, LEFT);
				sibling = node->parent->right;
			}
			if (siblingLeftChildColor == black && siblingRightChildColor == black) {
				sibling->color = red;
				node = node->parent;
			} else {
				if (siblingRightChildColor == black) {
					sibling->left->color = black;
					sibling->color = red;
					rotate(sibling, RIGHT);
					sibling = node->parent->right;
				}
				sibling->color = node->parent->color;
				node->parent->color = black;
				sibling->right->color = black;
				rotate(node->parent, LEFT);
				node = root;
			}
		} else {
			sibling = node->parent->left;
			siblingLeftChildColor = sibling->left != NULL ? sibling->left->color : black;
			siblingRightChildColor = sibling->right != NULL ? sibling->right->color : black;
			if (sibling->color == red) {
				sibling->color = black;
				node->parent->color = red;
				rotate(node->parent, RIGHT);
				sibling = node->parent->left;
			}
			if (siblingRightChildColor == black && siblingLeftChildColor == black) {
				sibling->color = red;
				node = node->parent;
			} else {
				if (siblingLeftChildColor == black) {
					sibling->right->color = black;
					sibling->color = red;
					rotate(sibling, LEFT);
					sibling = node->parent->left;
				}
				sibling->color = node->parent->color;
				node->parent->color = black;
				sibling->left->color = black;
				rotate(node->parent, RIGHT);
				node = root;
			}
		}
		if (node == root) break;
	}
	node->color = black;
}
