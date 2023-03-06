#include <iostream>
#include "AVLTreeBenchmark.h"

AVLTree* tree;

void init() {
}

void AVLTreeBenchmark::init(int) {
	tree = new AVLTree();
};
void AVLTreeBenchmark::insert(int key) {
	tree->insertKey(key);
}
void AVLTreeBenchmark::search(int key) {
	tree->searchKey(key);
}
void AVLTreeBenchmark::remove(int key) {
	tree->deleteKey(key);
}
void AVLTreeBenchmark::destroy() {
	delete tree;
}
