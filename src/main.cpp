// Your task in this assignment is to implement and then compare 4
// implementations of data structures in terms of the effectiveness of insert,
// delete and search operations in different situations:
//
// - (3 points) Implementation of a binary search tree (BVS) with any balancing
// algorithm, e.g. AVL, Red-Black Trees, (2,3) Trees, (2,3,4) Trees, Splay
// Trees, ...
// - (3 points) Second implementation of BVS with a different balancing
// algorithm than in the previous point.
// - (3 points) Implementation of a hash table with collision resolution of your
// choice. The hash table size adjustment must also be implemented.
// - (3 points) Second implementation of the hash table with collision
// resolution in a different way than in the previous point. The hash table size
// adjustment must also be implemented.

#include <iostream>
#include <sstream>
#include "gui.h"
#include "tests/test.h"
#include "gui/ViewController.h"

using namespace std;

// Add --test to the command line to run the tests
int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);

		if (arg == "--test") {
			testAVLTree();
			// testAVLTree2();
			// testHashTable();
			// testHashTable2();
			return 0;
		}
		return 0;
	}

	ViewController::run();

	return 0;
}