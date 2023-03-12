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

DSStandardWrapper::DSStandardWrapper() {
	this->name = "Standard Wrapper";
}
void DSStandardWrapper::init(int) {
}
void DSStandardWrapper::insert(int) {
}
int* DSStandardWrapper::getAllKeys(int& length) {
	return NULL;
}
int DSStandardWrapper::search(int) {
	return 0;
}
void DSStandardWrapper::returnlessSearch(int) {
}

void DSStandardWrapper::remove(int) {
}
bool DSStandardWrapper::isValid() {
	return true;
}
void DSStandardWrapper::destroy() {
}

DSStandardWrapper::~DSStandardWrapper() {
	destroy();
}