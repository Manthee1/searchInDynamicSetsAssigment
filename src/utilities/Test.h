#ifndef TEST_TEST_H_
#define TEST_TEST_H_
#include "utils.h"
#include "tests/AVLTreeTest.h"
#include <chrono>
#include <string>
#include <list>

// Structure for storing test data
struct TestEntry {
	std::string name;
	// Pointer to the class that will be tested
	DSStandardWrapper* test;
};

namespace Test {
	bool run(enum DataStructureType testType, int keysAmount);
	bool run(enum DataStructureType testType, int keysAmount, int amount);
};

#endif