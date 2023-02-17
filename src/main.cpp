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
#include "tests/test.cpp"
#include "modules/AVLTree.cpp"

using namespace std;

// Print tree with root at given level

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
		} else if (arg == "--asl") {
			// Read cin and create AVLTree
			AVLTree tree;
			// If the input starts with a number, then it is a key
			//  If it starts with d, then it is a delete command

			string line;
			cout << "Input: ";
			while (getline(cin, line)) {
				istringstream iss(line);
				string command;
				int key;
				iss >> command;
				if (command.at(0) == 'd') {
					key = stoi(command.substr(1));
					tree.deleteKey(key);
				} else {
					key = stoi(command);
					tree.insertKey(key);
				}

				cout << endl;

				printTree(tree.root, 0);
				cout << endl;
				cout << "Input: ";
			}
		}

		return 0;
	}
	return 0;
}