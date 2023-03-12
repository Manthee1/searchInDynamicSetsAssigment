#ifndef TEST_TEST_H_
#define TEST_TEST_H_
#include "../utils.h"
#include <chrono>
#include <string>
#include <list>

// Structure for storing test data
struct TestEntry {
	std::string name;
	// Pointer to the class that will be tested
	DSStandardWrapper* test;
};

enum TestType {
	BASIC,
	STRICT
};

namespace Test {
	bool run(DataStructureType dsType, TestType testType);
	bool run(DataStructureType dsType, TestType testType, std::string testFile);

};

#endif