#include <iostream>
#include "AVLTreeWrapper.h"

AVLTree* tree;

AVLTreeWrapper::AVLTreeWrapper() {
	this->name = "AVL Tree";
}

void AVLTreeWrapper::init(int) {
	tree = new AVLTree();
};
void AVLTreeWrapper::insert(int key) {
	tree->insertKey(key);
}
void AVLTreeWrapper::search(int key) {
	tree->searchKey(key);
}
void AVLTreeWrapper::remove(int key) {
	tree->deleteKey(key);
}
void AVLTreeWrapper::destroy() {
	delete tree;
}

AVLTreeWrapper::~AVLTreeWrapper() {
	delete tree;
}