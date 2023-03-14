#include <iostream>
#include <sstream>
#include <fstream>
#include "gui.h"
#include "utilities/WrappedDS.h"
#include "utilities/Test.h"
#include "utilities/Benchmark.h"
#include "gui/ViewController.h"

using namespace std;

DataStructureType getDataType(string dataTypeString) {
	// Get the dataStrcture type
	enum DataStructureType dataStrctureType = AVL;
	switch (dataTypeString[0]) {
	case 'a':
	case 'A':
		dataStrctureType = AVL;
		break;
	case 'r':
	case 'R':
		dataStrctureType = RedBlack;
		break;
	case 'o':
	case 'O':
		dataStrctureType = HashTableOpenAddressing;
		break;
	case 'c':
	case 'C':
		dataStrctureType = HashTableChaining;
		break;
	default:
		cout << "Invalid data strcture type provided (Expected: a, r, o, c)" << endl;
		cout << "Using AVL tree" << endl;
		return AVL;
	}
	return dataStrctureType;
}

void test(int argc, char** argv) {
	// If there is a second argument, use it as the data structure type
	if (argc < 3) {
		cout << "Please specify a valid test type (rand, fixed, gen)" << endl;
		cout << "Usage:" << endl;
		cout << " \ttest rand [a|r|o|c] [strict|basic]" << endl;
		cout << " \ttest fixed {filename} [a|r|o|c] [strict|basic]" << endl;
		cout << " \ttest gen {filename} {keysAmount1,keysAmount2,...} (keysAmount is the amount of keys for each test)" << endl;

		return;
	}

	string testTypeString = string(argv[2]);
	if (testTypeString == "rand") {
		// If there is a third argument, use it as the data structure type
		if (argc < 4) {
			cout << "Please specify a data structure type (a, r, o, c)" << endl;
			return;
		}
		string dsTypeString = string(argv[3]);
		// If there is a third argument, use it as the test type
		enum TestType testType = BASIC;
		if (argc > 4) {
			string testTypeString = string(argv[4]);
			if (testTypeString == "strict")
				testType = STRICT;
			else if (testTypeString != "basic")
				cout << YELLOW "INFO: Invalid test type, using basic" RESET << endl;
		} else
			cout << YELLOW "INFO: Test type not provided. Using basic test" RESET << endl;

		enum DataStructureType dsType = getDataType(dsTypeString);
		Test::run(dsType, testType);
		return;
	} else if (testTypeString == "fixed") {
		// Make sure that the file name is provided
		if (argc < 4) {
			cout << "Please specify a file name" << endl;
			return;
		}
		// Get the file name
		string fileName = string(argv[3]);
		// If there is a fourth argument, use it as the data structure type
		if (argc < 5) {
			cout << "Please specify a data structure type (a, r, o, c)" << endl;
			return;
		}
		string dsTypeString = string(argv[4]);
		// If there is a fifth argument, use it as the test type
		enum TestType testType = BASIC;
		if (argc > 5) {
			string testTypeString = string(argv[5]);
			if (testTypeString == "strict")
				testType = STRICT;
			else if (testTypeString != "basic")
				cout << YELLOW "INFO: Invalid test type, using basic" RESET << endl;
		} else
			cout << YELLOW "INFO: Test type not provided. Using basic test" RESET << endl;

		enum DataStructureType dsType = getDataType(dsTypeString);
		Test::run(dsType, testType, fileName);
		return;
	} else if (testTypeString == "gen") {
		// Make sure that the file name is provided
		if (argc < 4) {
			cout << "Please specify a file name" << endl;
			return;
		}
		// Get the file name
		string fileName = string(argv[3]);
		// Make sure that the amount of keys is provided
		if (argc < 5) {
			cout << "Please specify the amount of keys for each test (comma separated)" << endl;
			return;
		}

		// Check if the file already exists
		if (Test::testFileExists(fileName)) {
			cout << "File already exists. Do you want to overwrite it? (y/N): ";
			std::string input;
			std::getline(std::cin, input);

			// Detecte if enter is pressed
			if (input.empty() || (input[0] != 'y' && input[0] != 'Y')) {
				cout << "Aborting..." << endl;
				return;
			}
		}

		// Get the amount of keys
		string keysAmountString = string(argv[4]);
		// Split the string by ','
		stringstream ss(keysAmountString);
		string item;
		int keysAmount[100] = {0};
		int keysAmountSize = 0;
		while (getline(ss, item, ',')) {
			keysAmount[keysAmountSize] = atoi(item.c_str());
			keysAmountSize++;
		}
		// Generate the test file
		Test::generateTestFile(fileName, keysAmount, keysAmountSize);
		return;
	}

	cout << "Invalid test type provided" << endl;
	return;
}

void benchmark(int argc, char** argv) {
	// Make sure there is a second argument (the data structure type)
	if (argc < 3) {
		cout << "Please specify a benchmark type (a, r, o, c)" << endl;
		return;
	}

	// Get the benchmark type
	string benchmarkTypeString = string(argv[2]);
	enum DataStructureType benchmarkType = getDataType(benchmarkTypeString);

	// Get the type of randomization
	//  unique/u: all the keys are unique
	//  random/r: the keys are random
	string randomizationTypeString = string(argv[3]);
	enum RandomizationType randomizationType = UNIQUE_RANDOM;
	if ((argc > 3) && (randomizationTypeString == "random" || randomizationTypeString == "r"))
		randomizationType = NON_UNIQUE_RANDOM;

	// Get the amount of elements
	int elementsAmount = (argc > 4) ? atoi(argv[3]) : 1000;

	Benchmark::verboseLevel = 2;
	Benchmark::run(benchmarkType, randomizationType, elementsAmount, elementsAmount, elementsAmount, 10);

	return;
}

// Add --test to the command line to run the tests
int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);

		// If arg is run, run the GUI
		if (arg == "run") {
			// If there is a second argument, use it as the data structure type
			int viewType = AVL;
			if (argc < 3) {
				cout << "Please specify a data structure type (a, r, o, c)" << endl;
				cout << "Defaulting to AVL tree" << endl;
			} else
				viewType = getDataType(string(argv[2]));

			ViewController::run((ViewType)viewType);
		} else if (arg == "test")
			test(argc, argv);
		else if (arg == "benchmark")
			benchmark(argc, argv);
		else {
			cout << "Invalid argument provided" << endl;
			cout << "Usage: " << argv[0] << " [run|test|benchmark]" << endl;
		}
	} else
		ViewController::run();

	// Delete WrappedDS::DSentries
	for (int i = 0; i < 4; i++)
		delete WrappedDS::DSentries[i];

	return 0;
}
