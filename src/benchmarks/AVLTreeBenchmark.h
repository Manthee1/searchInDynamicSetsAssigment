#include "../utils.h"
#include "../modules/AVLTree.h"
#include <list>

class AVLTreeBenchmark : public BenchmarkTemplate {
private:
	AVLTree tree;

public:
	void insert(int key) {
		tree.insertKey(key);
	}
	void search(int key) {
		tree.searchKey(key);
	}
	void remove(int key) {
		tree.deleteKey(key);
	}
	void destroy() {
		tree.~AVLTree();
		tree = AVLTree();
	}
};
