#include "Benchmark.h"
#include <iostream>

BenchmarkData Benchmark::benchmarks[] = {
	{"AVL", new AVLTreeBenchmark()},
	{"RedBlack", NULL},
	{"HashTable (Chaining)", NULL},
	{"HashTable (OpenAddressing)", NULL}};

// Duration insert total static variable
std::chrono::nanoseconds insertTotal = std::chrono::nanoseconds(0);
std::chrono::nanoseconds searchTotal = std::chrono::nanoseconds(0);
std::chrono::nanoseconds removeTotal = std::chrono::nanoseconds(0);

void Benchmark::runInsertion(BenchmarkTemplate* benchmark, int insertKeys) {
	int* keys = generateRandomArray(insertKeys, 0, 1000000);

	// Start the timer
	auto start = std::chrono::high_resolution_clock::now();

	// Run the benchmark
	for (int i = 0; i < insertKeys; i++)
		benchmark->insert(keys[i]);

	// Stop the timer
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = stop - start;

	insertTotal += duration;

	// Print the results
	std::cout << "Inserted " << insertKeys << " keys in " << duration.count() << " seconds" << std::endl;
}

void Benchmark::runSearch(BenchmarkTemplate* benchmark, int searchKeys) {
	int* search = generateRandomArray(searchKeys, 0, 1000000);

	// Start the timer
	auto start = std::chrono::high_resolution_clock::now();

	// Run the benchmark
	for (int i = 0; i < searchKeys; i++)
		benchmark->search(search[i]);

	// Stop the timer
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = stop - start;

	searchTotal += duration;

	// Print the results
	std::cout << "Searched " << searchKeys << " keys in " << duration.count() << " seconds" << std::endl;
}

void Benchmark::runRemoval(BenchmarkTemplate* benchmark, int removeKeys) {
	// generate remove keys
	int* remove = generateRandomArray(removeKeys, 0, 1000000);

	// Start the timer
	auto start = std::chrono::high_resolution_clock::now();

	// Run the benchmark
	for (int i = 0; i < removeKeys; i++)
		benchmark->remove(remove[i]);

	// Stop the timer
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = stop - start;

	removeTotal += duration;

	// Print the results
	std::cout << "Removed " << removeKeys << " keys in " << duration.count() << " seconds" << std::endl;
}

void Benchmark::run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys) {
	BenchmarkData benchmarkData = benchmarks[benchmarkType];
	std::cout << "Running benchmark: " << benchmarkData.name << std::endl;
	// Run insertion
	runInsertion(benchmarkData.benchmark, insertKeys);

	// Run search
	runSearch(benchmarkData.benchmark, searchKeys);

	// Run removal
	runRemoval(benchmarkData.benchmark, removeKeys);
}

void Benchmark::run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys, int amount) {
	// Run the benchmark amount times
	for (int i = 0; i < amount; i++) {
		std::cout << "Run " << i + 1 << " of " << amount << std::endl;
		run(benchmarkType, insertKeys, searchKeys, removeKeys);
	}
	// Summarize the results
	std::cout << "Total time for " << amount << " runs:" << std::endl;
	std::cout << "Insertion: " << insertTotal.count() << " nanosecounds for " << insertKeys << " keys" << std::endl;
	std::cout << "Search: " << searchTotal.count() << " nanosecounds for" << searchKeys << " keys" << std::endl;
	std::cout << "Removal: " << removeTotal.count() << " nanosecounds for" << removeKeys << " keys" << std::endl;
}
