#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_
#include "benchmarks/AVLTreeBenchmark.h"
#include <chrono>
#include <string>
#include <list>

struct BenchmarkData {
	std::string name;
	class BenchmarkTemplate *benchmark;
};

enum BenchmarkType {
	AVL,
	RedBlack,
	HashTableChaining,
	HashTableOpenAddressing
};

namespace Benchmark {
	extern BenchmarkData benchmarks[];
	extern int benchmarksSize;
	void runInsertion(BenchmarkTemplate *benchmark, int insertKeys);
	void runSearch(BenchmarkTemplate *benchmark, int searchKeys);
	void runRemoval(BenchmarkTemplate *benchmark, int removeKeys);
	void run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys);
	void run(enum BenchmarkType benchmarkType, int insertKeys, int searchKeys, int removeKeys, int amount);
};

#endif