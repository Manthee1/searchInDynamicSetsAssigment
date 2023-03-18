#include <iostream>
#include <sstream>
#include <fstream>
#include "gui.h"
#include "utilities/WrappedDS.h"
#include "utilities/Test.h"
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

enum RandomizationType getRandType(string randTypeString) {
	// Get the randomization type
	enum RandomizationType randomizationType = UNIQUE_RANDOM;

	if (randTypeString == "random" || randTypeString == "r")
		randomizationType = NON_UNIQUE_RANDOM;
	else if (randTypeString != "unique" && randTypeString != "u")
		cout << YELLOW "INFO: Invalid randomization type, using unique" RESET << endl;
	return randomizationType;
}

enum TestType getBenchmarkType(string testTypeString) {
	enum TestType testType = BASIC;
	if (testTypeString == "strict")
		testType = STRICT;
	else if (testTypeString != "basic")
		cout << YELLOW "INFO: Invalid test type, using basic" RESET << endl;
	return testType;
}

void printTestHelp() {
	cout << "  test [rand|fixed|gen|benchmark] - Run a test on a data structure" << endl;
	cout << " \ttest rand [a|r|o|c] [strict|basic] [unique|random]" << endl;
	cout << " \ttest fixed {filename} [a|r|o|c] [strict|basic]" << endl;
	cout << " \ttest gen {filename} {keysAmount1,keysAmount2,...} [unique|random] (keysAmount is the amount of keys for each test)" << endl;
	cout << " \ttest [benchmark|bench|b] - Run a benchmark on a data structure" << endl;
	cout << " \t\tbenchmark rand [a|r|o|c] [unique|random] {keysAmount} {testAmount} - Runs a benchmark with random keys" << endl;
	cout << " \t\tbenchmark fixed {filename} [a|r|o|c] - Runs a benchmark with keys from a file" << endl;
}

void test(int argc, char** argv) {
	// If there is a second argument, use it as the data structure type
	if (argc < 3) {
		cout << "Please specify a valid test type (rand, fixed, gen)" << endl;
		printTestHelp();
		return;
	}

	string testTypeString = string(argv[2]);
	if (testTypeString == "rand") {
		// If there is a third argument, use it as the data structure type
		if (argc < 4) {
			cout << "Please specify a data structure type [a|r|o|c]" << endl;
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

		// Get the randomization type
		enum RandomizationType randomizationType = UNIQUE_RANDOM;
		if (argc > 5) {
			string randTypeString = string(argv[5]);
			randomizationType = getRandType(randTypeString);
		} else
			cout << YELLOW "INFO: Randomization type not provided. Using unique randomization" RESET << endl;

		enum DataStructureType dsType = getDataType(dsTypeString);
		Test::run(dsType, testType, randomizationType);
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
			cout << "Please specify a data structure type [a|r|o|c]" << endl;
			return;
		}
		string dsTypeString = string(argv[4]);
		// If there is a fifth argument, use it as the test type
		enum TestType testType = BASIC;
		if (argc > 5)
			testType = getBenchmarkType(string(argv[5]));
		else
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

		// Get the randomization type
		enum RandomizationType randomizationType = UNIQUE_RANDOM;
		if (argc > 5) {
			string randTypeString = string(argv[5]);
			randomizationType = getRandType(randTypeString);
		} else
			cout << YELLOW "INFO: Randomization type not provided. Using unique randomization" RESET << endl;

		// Generate the test file
		Test::generateTestFile(fileName, randomizationType, keysAmount, keysAmountSize);
		return;
	} else if (testTypeString == "benchmark" || testTypeString == "bench" || testTypeString == "b") {
		if (argc < 4) {
			cout << "Please specify a benchmark type (rand, fixed)" << endl;
			printTestHelp();
			return;
		}

		string benchmarkTypeString = string(argv[3]);

		if (benchmarkTypeString == "rand" || benchmarkTypeString == "r") {
			// Make sure there is a benchmark type
			if (argc < 5) {
				cout << "Please specify a data structure type [a|r|o|c]" << endl;
				printTestHelp();
				return;
			}

			// Get the benchmark type
			string dsTypeString = string(argv[4]);
			enum DataStructureType dsType = getDataType(dsTypeString);

			// Get the randomization type
			enum RandomizationType randomizationType = UNIQUE_RANDOM;
			if (argc > 5) {
				string randTypeString = string(argv[5]);
				randomizationType = getRandType(randTypeString);
			} else
				cout << YELLOW "INFO: Randomization type not provided. Using unique randomization" RESET << endl;

			// Get the element count
			int elementsAmount = (argc > 6) ? atoi(argv[6]) : 1000;
			if (argc <= 6) cout << YELLOW "INFO: Amount of elements not provided. Using " << elementsAmount << RESET << endl;

			// Get the test count
			int testCount = (argc > 7) ? atoi(argv[7]) : 10;
			if (argc <= 7) cout << YELLOW "INFO: Amount of tests not provided. Using " << testCount << RESET << endl;

			Test::Benchmark::verboseLevel = 2;
			Test::Benchmark::run(dsType, randomizationType, elementsAmount, testCount);

		} else if (benchmarkTypeString == "fixed" || benchmarkTypeString == "f") {
			// Test File
			if (argc < 5) {
				cout << "Please specify a test file" << endl;
				printTestHelp();
				return;
			}
			// make sure the file exists
			string fileName = string(argv[4]);
			if (!Test::testFileExists(fileName)) {
				cout << "Test file " << fileName << " does not exist" << endl;
				cout << "Please specify a valid test file or generate a new one" << endl;
				return;
			}

			// Data structure
			if (argc < 6) {
				cout << "Please specify a data structure type [a|r|o|c]" << endl;
				printTestHelp();
				return;
			}

			// Get the ds type
			string dsTypeString = string(argv[5]);
			enum DataStructureType dsType = getDataType(dsTypeString);

			Test::Benchmark::verboseLevel = 2;
			Test::Benchmark::run(dsType, fileName);
		} else {
			cout << "Please specify a valid benchmark type (rand, fixed)" << endl;
		}
		return;
	}
	cout << "Invalid test type provided" << endl;
	return;
}

void printHelp(string command) {
	cout << "Usage: " << command << " [run|test|benchmark]" << endl;
	cout << "Options:" << endl;
	cout << "  run [a|r|o|c]: run the GUI with the specified data structure type" << endl;
	cout << endl;
	printTestHelp();
}

int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);

		if (arg == "--help" || arg == "-h") {
			printHelp(string(argv[0]));
		}

		// If arg is run, run the GUI
		else if (arg == "run") {
			// If there is a second argument, use it as the data structure type
			int viewType = AVL;
			if (argc < 3) {
				cout << "Please specify a data structure type [a|r|o|c]" << endl;
				cout << "Defaulting to AVL tree" << endl;
			} else
				viewType = getDataType(string(argv[2]));

			ViewController::run((ViewType)viewType);
		} else if (arg == "test" || arg == "t")
			test(argc, argv);
		else {
			cout << "Invalid argument provided" << endl;
			printHelp(string(argv[0]));
		}
	} else {
		cout << "No argument provided" << endl;
		printHelp(string(argv[0]));
	}

	// Delete WrappedDS::DSentries
	for (int i = 0; i < 4; i++)
		delete WrappedDS::DSentries[i];

	return 0;
}
