#include "RedBlackTreeWrapper.h"

static RedBlackTree* tree;

RedBlackTreeWrapper::RedBlackTreeWrapper() {
	this->name = "RedBlackTree";
}

void RedBlackTreeWrapper::init(int) {
	tree = new RedBlackTree();
}

void RedBlackTreeWrapper::insert(int key) {
	tree->insertKey(key);
}

void RedBlackTreeWrapper::search(int key) {
	tree->searchKey(key);
}

void RedBlackTreeWrapper::remove(int key) {
	tree->deleteKey(key);
}

void RedBlackTreeWrapper::destroy() {
	delete tree;
}

RedBlackTreeWrapper::~RedBlackTreeWrapper() {
	delete tree;
}