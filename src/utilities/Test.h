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
	bool testFileExists(std::string filename);
	bool run(DataStructureType dsType, TestType testType, RandomizationType randType);
	bool run(DataStructureType dsType, TestType testType, std::string testFile);
	void generateTestFile(std::string testFile, RandomizationType randType, int* keysAmount, int keysAmountSize);
	namespace Benchmark {
		extern bool verbose;
		extern int verboseLevel;
		void run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys);
		void run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys, int amount);
	};
}
#endif