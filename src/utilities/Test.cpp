#include "Test.h"
#include "WrappedDS.h"

#include <iostream>

#define TESTS_AMOUNT 5
#define KEYS_PER_TEST 1000
static int keysAmounts[TESTS_AMOUNT] = {100, 1000, 10000, 100000, 1000000};

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

static bool runBasicTest(DSStandardWrapper* ds, int* keys, int keysAmount) {
	// Run Insertion
	std::cout << "Running insertion test ";
	for (int i = 0; i < keysAmount; i++)
		ds->insert(keys[i]);

	if (!verifyInsert(ds, keys, keysAmount)) return false;
	std::cout << GREEN "[PASSED]" RESET << std::endl;

	std::cout << "Running deletion test ";

	// Shuffle the keys and take half
	shuffleInts(keys, keysAmount);

	// Delete half of the keys
	for (int i = 0; i < keysAmount / 2; i++)
		ds->remove(keys[i]);

	// Verify deletion
	if (!verifyDelete(ds, keys, keysAmount / 2)) return false;

	std::cout << GREEN "[PASSED]" RESET << std::endl;

	// Shuffle the keys and take half
	shuffleInts(keys, keysAmount);

	// Rerun insertion
	std::cout << "Running insertion again test ";
	for (int i = 0; i < keysAmount / 2; i++)
		ds->insert(keys[i]);

	// Verify insertion
	if (!verifyInsert(ds, keys, keysAmount / 2)) return false;

	std::cout << GREEN "[PASSED]" RESET << std::endl;

	return true;
}

bool Test::run(DataStructureType dsType, TestType testType) {
	DSStandardWrapper* benchmarkData = WrappedDS::DSentries[dsType];

	// Running a testType test on a dsType data structure with keysAmount keys
	std::string testTypeString = testType == BASIC ? GREEN "basic" RESET : RED "strict" RESET;
	std::cout << "Running a " << testTypeString << " test on " << benchmarkData->name << std::endl;

	// Generate the keys
	for (int i = 0; i < TESTS_AMOUNT; i++) {
		std::cout << MAGENTA "====TEST " << i << "====" RESET << std::endl;
		std::cout << "Generating " YELLOW << keysAmounts[i] << RESET " keys" << std::endl;
		int* keys = generateRandomArray(keysAmounts[i], 0, keysAmounts[i] * 10);

		// Initialize the benchmark
		benchmarkData->init(keysAmounts[i]);
		// Run the test
		bool passed = runBasicTest(benchmarkData, keys, keysAmounts[i]);
		// Destroy the benchmark
		benchmarkData->destroy();

		std::string reslutString = passed ? GREEN "[Passed]" RESET : RED "[Failed]" RESET;
		std::cout << "Overall: " << reslutString << std::endl;
		std::cout << std::endl;
		delete[] keys;
	}
	return false;
}
bool run(DataStructureType dsType, TestType testType, std::string testFile) {
	return false;
}
