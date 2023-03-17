#include "globals.h"

// AVLNode constructors with key
AVLNode::AVLNode() {
	this->key = 0;
	this->value = 0;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->height = 1;
}

AVLNode::AVLNode(int key, int value) {
	this->key = key;
	this->value = value;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->height = 1;
}

RedBlackNode::RedBlackNode() {
	this->key = 0;
	this->value = 0;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->color = red;
}

// RedBlackNode constructors with key
RedBlackNode::RedBlackNode(int key, int value) {
	this->key = key;
	this->value = value;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->color = red;
}

// RedBlackNode constructor with key and color
RedBlackNode::RedBlackNode(int key, int value, RedBlackNodeColor color) {
	this->key = key;
	this->value = value;
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
int DSStandardWrapper::calculateSpaceComplexity() {
	return 0;
}
void DSStandardWrapper::destroy() {
}

DSStandardWrapper::~DSStandardWrapper() {
	destroy();
}