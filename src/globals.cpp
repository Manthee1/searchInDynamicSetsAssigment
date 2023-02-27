#include "globals.h"

// AVLNode constructors with key
AVLNode::AVLNode(int key) {
	this->key = key;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->height = 1;
}

// RedBlackNode constructors with key
RedBlackNode::RedBlackNode(int key) {
	this->key = key;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->color = red;
}

// RedBlackNode constructor with key and color
RedBlackNode::RedBlackNode(int key, RedBlackNodeColor color) {
	this->key = key;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->color = color;
}
