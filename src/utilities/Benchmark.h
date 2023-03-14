#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_
#include "../utils.h"

enum RandomizationType {
	UNIQUE_RANDOM,
	NON_UNIQUE_RANDOM,
};

namespace Benchmark {
	extern bool verbose;
	extern int verboseLevel;
	void run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys);
	void run(enum DataStructureType benchmarkType, RandomizationType randType, int insertKeys, int searchKeys, int removeKeys, int amount);
};

#endif