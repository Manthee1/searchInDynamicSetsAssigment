#ifndef TEST_TEST_H_
#define TEST_TEST_H_
#include "../utils.h"
#include <chrono>
#include <string>
#include <list>

namespace Test {
	bool testFileExists(std::string filename);
	bool run(DataStructureType dsType, TestType testType, RandomizationType randType);
	bool run(DataStructureType dsType, TestType testType, std::string testFile);
	void generateTestFile(std::string testFile, RandomizationType randType, int* keysAmount, int keysAmountSize, unsigned int);
	namespace Benchmark {
		extern bool verbose;
		extern int verboseLevel;
		void run(enum DataStructureType dsType, int* keys, int keysAmount);
		void run(enum DataStructureType dsType, RandomizationType randType, int keysAmount, int testAmount);
		void run(enum DataStructureType dsType, std::string testFile);
	};
}
#endif