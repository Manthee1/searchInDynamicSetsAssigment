#include <iostream>
#include "AVLTreeBenchmark.h"

AVLTree tree;

void init() {
}

void AVLTreeBenchmark::init() {
	tree = AVLTree();
};
void AVLTreeBenchmark::insert(int key) {
	tree.insertKey(key);
}
void AVLTreeBenchmark::search(int key) {
	tree.searchKey(key);
}
void AVLTreeBenchmark::remove(int key) {
	tree.deleteKey(key);
}
void AVLTreeBenchmark::destroy() {
	tree.~AVLTree();
	tree = AVLTree();
}

// AVLTreeBenchmark::AVLBenchmarkData = {
// 	"AVL Tree",
// 	&AVLTreeBenchmark::init,
// 	&AVLTreeBenchmark::insert,
// 	&AVLTreeBenchmark::search,
// 	&AVLTreeBenchmark::remove,
// 	&AVLTreeBenchmark::destroy};