#include "Benchmark.h"
#include "WrappedDS.h"
#include <iostream>
#include <chrono>
#include <string>
#include <list>

bool Benchmark::verbose = false;
int Benchmark::verboseLevel = 0;

std::chrono::nanoseconds insertTotal = std::chrono::nanoseconds(0);
std::chrono::nanoseconds searchTotal = std::chrono::nanoseconds(0);
std::chrono::nanoseconds removeTotal = std::chrono::nanoseconds(0);

std::chrono::nanoseconds runTest(DSStandardWrapper* benchmark, void (DSStandardWrapper::*benchmarkFunction)(int), int keysAmount) {
	int* keys = generateRandomArray(keysAmount, 0, keysAmount * 10);

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

void Benchmark::run(enum DataStructureType benchmarkType, int insertKeys, int searchKeys, int removeKeys) {
	DSStandardWrapper* benchmarkData = WrappedDS::DSentries[benchmarkType];
	if (verboseLevel > 1) std::cout << "Running benchmark: " << benchmarkData->name << std::endl;
	// Initialize the benchmark
	benchmarkData->init(insertKeys);

	// Run Insertion
	auto duration = runTest(benchmarkData, &DSStandardWrapper::insert, insertKeys);
	insertTotal += duration;
	// Print the results
	if (verboseLevel > 1) std::cout << "Inserted " GREEN << insertKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / insertKeys << " nanoseconds per key)" << std::endl;

	// Run Search
	duration = runTest(benchmarkData, &DSStandardWrapper::search, searchKeys);
	searchTotal += duration;
	// Print the results
	if (verboseLevel > 1) std::cout << "Searched " GREEN << searchKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / searchKeys << " nanoseconds per key)" << std::endl;

	// Run Removal
	duration = runTest(benchmarkData, &DSStandardWrapper::remove, removeKeys);
	removeTotal += duration;

	// Print the results
	if (verboseLevel > 1) std::cout << "Removed " GREEN << removeKeys << RESET " keys in " YELLOW << duration.count() << RESET " nanoseconds (" << duration.count() / removeKeys << " nanoseconds per key)" << std::endl;

	// Destrucor
	benchmarkData->destroy();
}

void Benchmark::run(enum DataStructureType benchmarkType, int insertKeys, int searchKeys, int removeKeys, int amount) {
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
		run(benchmarkType, insertKeys, searchKeys, removeKeys);
	}
	if (verboseLevel > 0) {
		// Summarize the results
		std::cout << std::endl;
		std::cout << "Total time for " << amount << " runs:" << std::endl;
		std::cout << "Insertion: " BOLD YELLOW << insertTotal.count() << RESET " nanoseconds for " GREEN << insertKeys << RESET " keys" << std::endl;
		std::cout << "Search: " BOLD YELLOW << searchTotal.count() << RESET " nanoseconds for " GREEN << searchKeys << RESET " keys" << std::endl;
		std::cout << "Removal: " BOLD YELLOW << removeTotal.count() << RESET " nanoseconds for " GREEN << removeKeys << RESET " keys" << std::endl;

		// Insertion average
		std::cout << std::endl;
		std::cout << "Insertion average: " BOLD YELLOW << insertTotal.count() / amount << RESET " nanoseconds per run" << std::endl;
		std::cout << "Insertion average per key: " BOLD MAGENTA << insertTotal.count() / (amount * insertKeys) << RESET " nanoseconds per key" << std::endl;

		// Search average
		std::cout << std::endl;
		std::cout << "Search average: " BOLD YELLOW << searchTotal.count() / amount << RESET " nanoseconds per run" << std::endl;
		std::cout << "Search average per key: " BOLD MAGENTA << searchTotal.count() / (amount * searchKeys) << RESET " nanoseconds per key" << std::endl;

		// Removal average
		std::cout << std::endl;
		std::cout << "Removal average: " BOLD YELLOW << removeTotal.count() / amount << RESET " nanoseconds per run or " << removeTotal.count() / 1000 / amount << " microseconds per run" << std::endl;
		std::cout << "Removal average per key: " BOLD MAGENTA << removeTotal.count() / (amount * removeKeys) << RESET " nanoseconds per key" << std::endl;
	}

	// Reset the totals
	insertTotal = std::chrono::nanoseconds(0);
	searchTotal = std::chrono::nanoseconds(0);
	removeTotal = std::chrono::nanoseconds(0);
}
