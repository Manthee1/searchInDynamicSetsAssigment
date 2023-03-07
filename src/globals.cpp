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

void DSStandardWrapper::init(int) {
}
void DSStandardWrapper::insert(int) {
}
bool verify() {
	return true;
}
int* getAllKeys(int&) {
	return NULL;
}
void DSStandardWrapper::search(int) {
}
void DSStandardWrapper::remove(int) {
}
void DSStandardWrapper::destroy() {
}
