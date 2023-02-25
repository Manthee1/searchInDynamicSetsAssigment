#include <iostream>
#include <sstream>
#include "gui.h"
#include "tests/test.h"
#include "Benchmark.h"
#include "gui/ViewController.h"

using namespace std;

// Add --test to the command line to run the tests
int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);

		if (arg == "--test") {
			testAVLTree(true);
			// testAVLTree2();
			// testHashTable();
			// testHashTable2();
			return 0;
		}
		if (arg == "--benchmark") {
			Benchmark::run(AVL, 1000, 800, 500, 10);
			return 0;
		}
		return 0;
	}

	ViewController::run();

	return 0;
}