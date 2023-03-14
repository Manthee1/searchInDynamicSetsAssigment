#include "Benchmark.h"
#include "WrappedDS.h"
#include <iostream>
#include <chrono>
#include <string>
#include <list>

bool Benchmark::verbose = false;
int Benchmark::verboseLevel = 0;

std::chrono::nanoseconds insertTotalTime = std::chrono::nanoseconds(0);
std::chrono::nanoseconds searchTotalTime = std::chrono::nanoseconds(0);
std::chrono::nanoseconds removeTotalTime = std::chrono::nanoseconds(0);
int spaceComplexityTotal = 0;

static std::chrono::nanoseconds runBenchmark(DSStandardWrapper* benchmark, void (DSStandardWrapper::*benchmarkFunction)(int), RandomizationType randType, int keysAmount) {
	int* keys = randType == NON_UNIQUE_RANDOM ? generateRandomArray(keysAmount, 0, keysAmount * 10) : generateRandomUniqueArray(keysAmount, 0);

	// Start the timer
	auto start = std::chrono::high_resolution_clock::now();

	// Run the benchmark
	for (int i = 0; i < keysAmount; i++)
		(benchmark->*benchmarkFunction)(keys[i]);

	// Stop the timer
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = stop - start;

	delete[] keys;
	return duration;
}

void Benchmark::run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys) {
	DSStandardWrapper* benchmarkData = WrappedDS::DSentries[benchmarkType];
	if (verboseLevel > 1) std::cout << "Running benchmark: " << benchmarkData->name << std::endl;
	// Initialize the benchmark
	benchmarkData->init(insertKeys);

	// Run Insertion
	auto duration = runBenchmark(benchmarkData, &DSStandardWrapper::insert, randType, insertKeys);
	insertTotalTime += duration;
	// Print the results
	if (verboseLevel > 1) std::cout << "Inserted " GREEN << insertKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / insertKeys << " nanoseconds per key)" << std::endl;

	// Save space complexity
	int spaceComplexity = benchmarkData->calculateSpaceComplexity();
	spaceComplexityTotal += spaceComplexity;

	// Run Search
	duration = runBenchmark(benchmarkData, &DSStandardWrapper::returnlessSearch, randType, searchKeys);
	searchTotalTime += duration;
	// Print the results
	if (verboseLevel > 1) std::cout << "Searched " GREEN << searchKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / searchKeys << " nanoseconds per key)" << std::endl;

	// Run Removal
	duration = runBenchmark(benchmarkData, &DSStandardWrapper::remove, randType, removeKeys);
	removeTotalTime += duration;

	// Print the results
	if (verboseLevel > 1) std::cout << "Removed " GREEN << removeKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / removeKeys << " nanoseconds per key)" << std::endl;

	// SpaceComplexity
	if (verboseLevel > 1) std::cout << "Space complexity: " << spaceComplexity << std::endl;

	// Destrucor
	benchmarkData->destroy();
}

void Benchmark::run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys, int amount) {
	if (insertKeys <= 0 || searchKeys <= 0 || removeKeys <= 0 || amount <= 0) {
		std::cout << "Error: Invalid argument" << std::endl;
		return;
	}
	// Run the benchmark amount times
	for (int i = 0; i < amount; i++) {
		if (verboseLevel > 1) {
			std::cout << std::endl;
			std::cout << "Run " << i + 1 << " of " << amount << std::endl;
		}
		run(benchmarkType, randType, insertKeys, searchKeys, removeKeys);
	}
	if (verboseLevel > 0) {
		// Summarize the results
		std::cout << std::endl;
		std::cout << "Total time for " << amount << " runs:" << std::endl;
		std::cout << "Insertion: " BOLD YELLOW << insertTotalTime.count() << RESET " nanoseconds for " GREEN << insertKeys << RESET " keys" << std::endl;
		std::cout << "Search: " BOLD YELLOW << searchTotalTime.count() << RESET " nanoseconds for " GREEN << searchKeys << RESET " keys" << std::endl;
		std::cout << "Removal: " BOLD YELLOW << removeTotalTime.count() << RESET " nanoseconds for " GREEN << removeKeys << RESET " keys" << std::endl;

		// Insertion average
		std::cout << std::endl;
		std::cout << "Insertion average: " BOLD YELLOW << insertTotalTime.count() / amount << RESET " nanoseconds per run" << std::endl;
		std::cout << "Insertion average per key: " BOLD MAGENTA << insertTotalTime.count() / (amount * insertKeys) << RESET " nanoseconds per key" << std::endl;

		// Search average
		std::cout << std::endl;
		std::cout << "Search average: " BOLD YELLOW << searchTotalTime.count() / amount << RESET " nanoseconds per run" << std::endl;
		std::cout << "Search average per key: " BOLD MAGENTA << searchTotalTime.count() / (amount * searchKeys) << RESET " nanoseconds per key" << std::endl;

		// Removal average
		std::cout << std::endl;
		std::cout << "Removal average: " BOLD YELLOW << removeTotalTime.count() / amount << RESET " nanoseconds per run or " << removeTotalTime.count() / 1000 / amount << " microseconds per run" << std::endl;
		std::cout << "Removal average per key: " BOLD MAGENTA << removeTotalTime.count() / (amount * removeKeys) << RESET " nanoseconds per key" << std::endl;

		// Space complexity
		std::cout << std::endl;
		std::cout << "Space complexity average: " BOLD YELLOW << spaceComplexityTotal / amount << RESET " bytes" << std::endl;
		std::cout << "Space complexity average per key: " BOLD MAGENTA << spaceComplexityTotal / (amount * insertKeys) << RESET " bytes per key" << std::endl;
	}

	// Reset the totals
	insertTotalTime = std::chrono::nanoseconds(0);
	searchTotalTime = std::chrono::nanoseconds(0);
	removeTotalTime = std::chrono::nanoseconds(0);
	spaceComplexityTotal = 0;
}
