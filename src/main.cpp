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
			// Check if verbose
			if (argc > 2) {
				string arg2 = string(argv[2]);
				if (arg2 == "--verbose")
					Benchmark::verboseLevel = 2;
			}
			Benchmark::run(AVL, 10000000, 10000000, 10000000, 10);
			// for (int i = 1; i <= 1000; i++) {
			// 	int* durations = Benchmark::run(AVL, i * 10, i * 10, i * 10, 1000);
			// 	// cout << "AVL Tree: " << i * 10 << " keys Duration: " << durations[0] << " nanoseconds" << endl;
			// 	cout << i << "\t" << durations[0] << "\t" << durations[1] << "\t" << durations[2] << "\t" << endl;
			// }
			return 0;
		}
		return 0;
	}

	ViewController::run();

	return 0;
}
