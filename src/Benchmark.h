#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_
#include "benchmarks/AVLTreeBenchmark.h"
#include <chrono>
#include <string>
#include <list>

enum BenchmarkType {
	AVL,
	RedBlack,
	HashTableChaining,
	HashTableOpenAddressing
};

namespace Benchmark {
	extern bool verbose;
	extern int verboseLevel;
	extern BenchmarkData benchmarks[];
	extern int benchmarksSize;
	void runInsertion(BenchmarkData benchmark, int insertKeys);
	void runSearch(BenchmarkData benchmark, int searchKeys);
	void runRemoval(BenchmarkData benchmark, int removeKeys);
	// Return
	void run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys);
	int* run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys, int amount);
};

#endif