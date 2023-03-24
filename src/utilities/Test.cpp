#include "Test.h"
#include "WrappedDS.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define TEST_DIR ""
#define TEST_FILE_EXT ".dsdata"

#define BENCHMARK_FINAL_TESTS_AMOUNT 10	 // how many times we should insert a the final key to get a good average

#define TESTS_AMOUNT 5
#define KEYS_PER_TEST 1000
static int keysAmounts[TESTS_AMOUNT] = {100, 1000, 10000, 100000, 1000000};

bool Test::Benchmark::verbose = false;
int Test::Benchmark::verboseLevel = 0;

DSStandardWrapper* dsClone = nullptr;

std::chrono::nanoseconds insertTotalTime = std::chrono::nanoseconds(0);
std::chrono::nanoseconds searchTotalTime = std::chrono::nanoseconds(0);
std::chrono::nanoseconds removeTotalTime = std::chrono::nanoseconds(0);
long long spaceComplexityTotal = 0;

bool Test::testFileExists(std::string filename) {
	filename = TEST_DIR + filename + TEST_FILE_EXT;
	std::ifstream file(filename);
	return file.good();
}

static int* getTestKeysFromFile(std::ifstream& file, int testNum, int& length) {
	// Go to beginning of file
	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	while (std::getline(file, line)) {
		// Check if the line starts with the given test number
		if (line.find(std::to_string(testNum) + "-") == 0) {
			// Extract the keysAmount and keys from the line
			std::stringstream ss(line);
			std::string testHeader, keysStr;
			std::getline(ss, testHeader, ':');
			std::getline(ss, keysStr);

			// Convert keysAmount string to int
			int keysAmount = std::stoi(testHeader.substr(testHeader.find('-') + 1));

			// Create array and parse keys
			int* keys = new int[keysAmount];
			std::stringstream keysSS(keysStr);
			for (int i = 0; i < keysAmount; i++) {
				keysSS >> keys[i];
			}

			// Set the length and return the array
			length = keysAmount;
			return keys;
		}
	}

	// Test not found
	std::cerr << "Test " << testNum << " not found in file." << std::endl;
	return nullptr;
}

static bool verifyInsert(DSStandardWrapper* ds, int* keys, int keysAmount) {
	if (!ds->isValid()) {
		std::cout << RED "[FAILED]" RESET << std::endl;
		std::cout << "Test failed: " << ds->name << " Data structure is not valid after insertion" << std::endl;
		return false;
	}
	// Check if all keys are inserted
	for (int i = 0; i < keysAmount; i++) {
		// Check if key is in the data structure
		if (ds->search(keys[i]) == -1) {
			std::cout << RED "[FAILED]" RESET << std::endl;
			std::cout << "Test failed: " << ds->name << " Key " << keys[i] << " is not in the data structure" << std::endl;
			return false;
		}
	}
	// Check if too much keys were inserted
	if (ds->getSize() > keysAmount) {
		std::cout << RED "[FAILED]" RESET << std::endl;
		std::cout << "Test failed: " << ds->name << " has more keys inserted than there should be (" << ds->getSize() << "/" << keysAmount << ")" << std::endl;
		return false;
	}

	return true;
}

static bool verifyDelete(DSStandardWrapper* ds, int* keys, int keysAmount) {
	if (!ds->isValid()) {
		std::cout << RED "[FAILED]" RESET << std::endl;
		std::cout << "Test failed: " << ds->name << " Data structure is not valid after deletion" << std::endl;
		return false;
	}
	// Check if all keys are deleted
	for (int i = 0; i < keysAmount; i++) {
		// Check if key is in the data structure
		if (ds->search(keys[i]) != -1) {
			std::cout << RED "[FAILED]" RESET << std::endl;
			std::cout << "Test failed: " << ds->name << " Key " << keys[i] << " is still in the data structure" << std::endl;
			return false;
		}
	}
	return true;
}

static bool runTest(DSStandardWrapper* ds, TestType testType, int* keys, int keysAmount) {
	// Run Insertion
	std::cout << "Running insertion test ";
	std::cout.flush();
	for (int i = 0; i < keysAmount; i++) {
		ds->insert(keys[i]);
		if (testType == STRICT && !verifyInsert(ds, &keys[i], 1)) {
			std::cout << "Failed on key " << keys[i] << std::endl;
			return false;
		}
	}

	if (!verifyInsert(ds, keys, keysAmount)) return false;
	std::cout << GREEN "[PASSED]" RESET << std::endl;

	// Shuffle the keys and take half
	shuffleInts(keys, keysAmount);

	// Delete half of the keys
	std::cout << "Running deletion test ";
	std::cout.flush();
	for (int i = 0; i < keysAmount / 2; i++) {
		ds->remove(keys[i]);
		if (testType == STRICT && !verifyDelete(ds, &keys[i], 1)) {
			std::cout << "Failed on key " << keys[i] << std::endl;
			return false;
		}
	}

	// Verify deletion
	if (!verifyDelete(ds, keys, keysAmount / 2)) return false;

	std::cout << GREEN "[PASSED]" RESET << std::endl;

	// Shuffle the keys and take half
	shuffleInts(keys, keysAmount);

	// Rerun insertion
	std::cout << "Re-running insertion test ";
	std::cout.flush();
	for (int i = 0; i < keysAmount / 2; i++) {
		ds->insert(keys[i]);
		if (testType == STRICT && !verifyInsert(ds, &keys[i], 1)) {
			std::cout << "Failed on key " << keys[i] << std::endl;
			return false;
		}
	}

	// Verify insertion
	if (!verifyInsert(ds, keys, keysAmount / 2)) return false;

	std::cout << GREEN "[PASSED]" RESET << std::endl;

	return true;
}

static bool run(DataStructureType dsType, TestType testType, int* keys, int keysAmount, int testNum) {
	DSStandardWrapper* ds = WrappedDS::DSentries[dsType];

	std::cout << MAGENTA "====TEST " << testNum << "====" RESET << std::endl;
	// Initialize the benchmark
	ds->init(keysAmount);
	// Run the test
	bool passed = testType == BASIC ? runTest(ds, testType, keys, keysAmount) : runTest(ds, testType, keys, keysAmount);
	// Destroy the benchmark
	ds->destroy();

	std::string reslutString = passed ? GREEN "[Passed]" RESET : RED "[Failed]" RESET;
	std::cout << "Overall: " << reslutString << std::endl;
	std::cout << std::endl;
	return passed;
}

bool Test::run(DataStructureType dsType, TestType testType, RandomizationType randType) {
	DSStandardWrapper* ds = WrappedDS::DSentries[dsType];
	bool ret = true;
	// Running a testType test on a dsType data structure with keysAmount keys
	std::string testTypeString = testType == BASIC ? GREEN "basic" RESET : RED "strict" RESET;
	std::cout << "Running a radnomly generated " << testTypeString << " test on " << ds->name << std::endl;
	for (int i = 0; i < TESTS_AMOUNT; i++) {
		std::cout << "Generating " YELLOW << keysAmounts[i] << RESET " keys" << std::endl;
		int* keys = randType == NON_UNIQUE_RANDOM ? generateRandomArray(keysAmounts[i], 0, keysAmounts[i] * 10) : generateRandomUniqueArray(keysAmounts[i], 0);
		// Run the test
		ret = run(dsType, testType, keys, keysAmounts[i], i) ? ret : false;
		delete[] keys;
	}
	return ret;
}

bool Test::run(DataStructureType dsType, TestType testType, std::string testFile) {
	DSStandardWrapper* ds = WrappedDS::DSentries[dsType];
	bool ret = true;
	// Running a testType test on a dsType data structure with keysAmount keys
	std::string testTypeString = testType == BASIC ? GREEN "basic" RESET : RED "strict" RESET;
	std::cout << "Running a test from file " << testFile << " on " << ds->name << std::endl;
	std::ifstream file;
	testFile = TEST_DIR + testFile + TEST_FILE_EXT;
	file.open(testFile);
	if (!file.is_open()) {
		std::cout << "Failed to open file " << testFile << std::endl;
		return false;
	}

	// Get the number of lines
	int lines = 0;
	std::string line;
	while (std::getline(file, line)) lines++;

	// Go back to the beginning of the file
	file.clear();

	for (int i = 0; i < lines; i++) {
		// Get the keys amount
		int keysAmount = 0;
		// Get the keys
		int* keys = getTestKeysFromFile(file, i + 1, keysAmount);
		// Run the test
		ret = run(dsType, testType, keys, keysAmount, i + 1) ? ret : false;
		delete[] keys;
	}
	return ret;
}

void Test::generateTestFile(std::string testFile, RandomizationType randType, int* keysAmount, int keysAmountSize) {
	// Running a testType test on a dsType data structure with keysAmount keys
	std::cout << "Generating a test file with " << keysAmountSize << " tests with the following keys amounts: " << std::endl;
	for (int i = 0; i < keysAmountSize; i++) {
		std::cout << keysAmount[i] << " ";
	}
	std::cout << std::endl;
	srand(time(NULL));

	testFile = TEST_DIR + testFile + TEST_FILE_EXT;

	// Open the file (If the file is not empty, delete the contents)
	std::ofstream file(testFile, std::ofstream::out | std::ofstream::trunc);
	if (!file.is_open()) {
		std::cout << "Failed to open file " << testFile << std::endl;
		return;
	}

	// Generate the keys
	for (int i = 0; i < keysAmountSize; i++) {
		std::cout << "Generating " YELLOW << keysAmount[i] << RESET " keys for test " MAGENTA << i + 1 << RESET << std::endl;
		int* keys = randType == NON_UNIQUE_RANDOM ? generateRandomArray(keysAmount[i], 0, keysAmount[i] * 10) : generateRandomUniqueArray(keysAmount[i], 0);

		// Write the keys to the file
		file << i + 1 << "-" << keysAmount[i] << ":";
		for (int j = 0; j < keysAmount[i]; j++)
			file << keys[j] << " ";
		file << std::endl;

		delete[] keys;
	}
	file.close();
}

// ==================== Benchmark ====================

static std::chrono::nanoseconds runBenchmark(DSStandardWrapper* ds, void (DSStandardWrapper::*dsFunction)(int), int* keys, int keysAmount) {
	// Start the timer
	auto start = std::chrono::high_resolution_clock::now();

	// Run the benchmark
	for (int i = 0; i < keysAmount; i++) {
		int key = keys[i];
		(ds->*dsFunction)(keys[i]);
	}

	// Stop the timer
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = stop - start;

	return duration;
}

void Test::Benchmark::run(enum DataStructureType dsType, int* keys, int keysAmount) {
	DSStandardWrapper* ds = WrappedDS::DSentries[dsType];
	if (Test::Benchmark::verboseLevel > 1) std::cout << "Running benchmark: " << ds->name << std::endl;

	// Initialize the benchmark
	ds->init(keysAmount);

	// Run Insertion
	auto duration = runBenchmark(ds, &DSStandardWrapper::insert, keys, keysAmount);
	insertTotalTime += duration;
	// Print the results
	if (Test::Benchmark::verboseLevel > 1) std::cout << "Inserted " GREEN << keysAmount << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / keysAmount << " nanoseconds per key)" << std::endl;

	// Save space complexity
	long long spaceComplexity = ds->calculateSpaceComplexity();
	spaceComplexityTotal += spaceComplexity;

	// Run Search
	duration = runBenchmark(ds, &DSStandardWrapper::returnlessSearch, keys, keysAmount);
	searchTotalTime += duration;
	// Print the results
	if (Test::Benchmark::verboseLevel > 1) std::cout << "Searched " GREEN << keysAmount << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / keysAmount << " nanoseconds per key)" << std::endl;

	// Run Removal
	duration = runBenchmark(ds, &DSStandardWrapper::remove, keys, keysAmount);
	removeTotalTime += duration;

	// Print the results
	if (Test::Benchmark::verboseLevel > 1) std::cout << "Removed " GREEN << keysAmount << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / keysAmount << " nanoseconds per key)" << std::endl;

	// SpaceComplexity
	if (Test::Benchmark::verboseLevel > 1) std::cout << "Space complexity: " MAGENTA << spaceComplexity << RESET " bytes (" << spaceComplexity / keysAmount << " bytes per key)" << std::endl;

	// Destrucor
	ds->destroy();
}
void printResluts(int keysAmount, int testAmount) {
	// Summarize the results
	std::cout << std::endl;
	std::cout << "Total time for " << testAmount << " runs:" << std::endl;
	std::cout << "Insertion: " BOLD YELLOW << insertTotalTime.count() << RESET " nanoseconds for " GREEN << keysAmount << RESET " keys" << std::endl;
	std::cout << "Search: " BOLD YELLOW << searchTotalTime.count() << RESET " nanoseconds for " GREEN << keysAmount << RESET " keys" << std::endl;
	std::cout << "Removal: " BOLD YELLOW << removeTotalTime.count() << RESET " nanoseconds for " GREEN << keysAmount << RESET " keys" << std::endl;

	// Insertion average
	std::cout << std::endl;
	std::cout << "Insertion average: " BOLD YELLOW << insertTotalTime.count() / testAmount << RESET " nanoseconds per run" << std::endl;
	std::cout << "Insertion average per key: " BOLD MAGENTA << insertTotalTime.count() / (testAmount * keysAmount) << RESET " nanoseconds per key" << std::endl;

	// Search average
	std::cout << std::endl;
	std::cout << "Search average: " BOLD YELLOW << searchTotalTime.count() / testAmount << RESET " nanoseconds per run" << std::endl;
	std::cout << "Search average per key: " BOLD MAGENTA << searchTotalTime.count() / (testAmount * keysAmount) << RESET " nanoseconds per key" << std::endl;

	// Removal average
	std::cout << std::endl;
	std::cout << "Removal average: " BOLD YELLOW << removeTotalTime.count() / testAmount << RESET " nanoseconds per run or " << removeTotalTime.count() / 1000 / testAmount << " microseconds per run" << std::endl;
	std::cout << "Removal average per key: " BOLD MAGENTA << removeTotalTime.count() / (testAmount * keysAmount) << RESET " nanoseconds per key" << std::endl;

	// Space complexity
	std::cout << std::endl;
	std::cout << "Space complexity average: " BOLD YELLOW << spaceComplexityTotal / testAmount << RESET " bytes" << std::endl;
	std::cout << "Space complexity average per key: " BOLD MAGENTA << spaceComplexityTotal / (testAmount * keysAmount) << RESET " bytes per key" << std::endl;
}

void Test::Benchmark::run(enum DataStructureType dsType, RandomizationType randType, int keysAmount, int testAmount) {
	if (keysAmount <= 0 || testAmount <= 0) {
		std::cout << "Error: Invalid argument" << std::endl;
		return;
	}
	std::cout << "Starting a radnomized " << (randType ? "non-unique" : "unique") << " benchmark of " << keysAmount << " elements for " << WrappedDS::DSentries[dsType]->name << std::endl;

	std::cout << "Running " << testAmount << " times" << std::endl;

	// Run the benchmark amount times
	for (int i = 0; i < testAmount; i++) {
		if (Test::Benchmark::verboseLevel > 1) {
			std::cout << std::endl;
			std::cout << "Run " << i + 1 << " of " << testAmount << std::endl;
		}
		int* keys = randType == NON_UNIQUE_RANDOM ? generateRandomArray(keysAmount, 0, keysAmount * 10) : generateRandomUniqueArray(keysAmount, 0);
		run(dsType, keys, keysAmount);
		delete[] keys;
	}
	if (Test::Benchmark::verboseLevel > 0) {
		printResluts(keysAmount, testAmount);
	}

	// Reset the totals
	insertTotalTime = std::chrono::nanoseconds(0);
	searchTotalTime = std::chrono::nanoseconds(0);
	removeTotalTime = std::chrono::nanoseconds(0);
	spaceComplexityTotal = 0;
}

void Test::Benchmark::run(enum DataStructureType dsType, std::string testFile) {
	DSStandardWrapper* ds = WrappedDS::DSentries[dsType];
	bool ret = true;
	// Running a testType test on a dsType data structure with keysAmount keys

	std::ifstream file;
	testFile = TEST_DIR + testFile + TEST_FILE_EXT;
	file.open(testFile);
	if (!file.is_open()) {
		std::cout << "Failed to open file " << testFile << std::endl;
		return;
	}

	// Get the number of lines
	int lines = 0;
	std::string line;
	while (std::getline(file, line)) lines++;

	// Go back to the beginning of the file
	file.clear();

	std::cout << "Starting a fixed (" << testFile << ") benchmark for " << WrappedDS::DSentries[dsType]->name << std::endl;

	std::cout << "Running " << lines << " times" << std::endl;

	int totalKeysAmount = 0;
	for (int i = 0; i < lines; i++) {
		if (Test::Benchmark::verboseLevel > 1) {
			std::cout << std::endl;
			std::cout << "Run " << i + 1 << " of " << lines << std::endl;
		}
		// Get the keys amount
		int testKeysAmount = 0;
		// Get the keys
		int* keys = getTestKeysFromFile(file, i + 1, testKeysAmount);

		totalKeysAmount += testKeysAmount;
		// Run the test
		run(dsType, keys, testKeysAmount);
		delete[] keys;
	}

	if (Test::Benchmark::verboseLevel > 0) {
		printResluts(totalKeysAmount, lines);
	}

	// Reset the totals
	insertTotalTime = std::chrono::nanoseconds(0);
	searchTotalTime = std::chrono::nanoseconds(0);
	removeTotalTime = std::chrono::nanoseconds(0);
	spaceComplexityTotal = 0;
}