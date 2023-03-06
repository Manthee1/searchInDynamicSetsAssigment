#include "RedBlackTreeBenchmark.h"

static RedBlackTree tree;

void RedBlackTreeBenchmark::init(int) {
	tree = RedBlackTree();
}

void RedBlackTreeBenchmark::insert(int key) {
	tree.insertKey(key);
}

void RedBlackTreeBenchmark::search(int key) {
	tree.searchKey(key);
}

void RedBlackTreeBenchmark::remove(int key) {
	tree.deleteKey(key);
}

void RedBlackTreeBenchmark::destroy() {
	// tree
}
