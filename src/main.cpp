#include <iostream>
#include <sstream>
#include "gui.h"
#include "utilities/WrappedDS.h"
#include "utilities/Test.h"
#include "utilities/Benchmark.h"
#include "gui/ViewController.h"

using namespace std;

DataStructureType getDataType(string dataTypeString) {
	// Get the benchmark type
	enum DataStructureType benchmarkType = AVL;
	switch (dataTypeString[0]) {
	case 'a':
	case 'A':
		benchmarkType = AVL;
		break;
	case 'r':
	case 'R':
		benchmarkType = RedBlack;
		break;
	case 'o':
	case 'O':
		benchmarkType = HashTableOpenAddressing;
		break;
	case 'c':
	case 'C':
		benchmarkType = HashTableChaining;
		break;
	default:
		cout << "Invalid benchmark type" << endl;
		return AVL;
	}
	return benchmarkType;
}

void test(int argc, char** argv) {
	// If there is a second argument, use it as the test type
	if (argc > 2) {
		string testTypeString = string(argv[2]);
		enum DataStructureType testType = getDataType(testTypeString);
		Test::run(testType, 1000, 100);
		return;
	}
	return;
}

void benchmark(int argc, char** argv) {
	// Make sure there is a second argument (the benchmark type)
	if (argc < 3) {
		cout << "Please specify a benchmark type" << endl;
		return;
	}

	// Get the benchmark type
	string benchmarkTypeString = string(argv[2]);
	enum DataStructureType benchmarkType = getDataType(benchmarkTypeString);

	int elementsAmount = (argc > 3) ? atoi(argv[3]) : 1000;

	Benchmark::verboseLevel = 2;
	Benchmark::run(benchmarkType, elementsAmount, elementsAmount, elementsAmount, 5);

	return;
}

// Add --test to the command line to run the tests
int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);
		if (arg == "--test")
			test(argc, argv);
		else if (arg == "--benchmark")
			benchmark(argc, argv);
	} else
		ViewController::run();

	// Delete WrappedDS::DSentries
	for (int i = 0; i < 4; i++)
		delete WrappedDS::DSentries[i];

	return 0;
}
