#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_
#include "../utils.h"

enum TestType {
	INSERT,
	SEARCH,
	REMOVE
};

namespace Benchmark {
	extern bool verbose;
	extern int verboseLevel;
	void runInsertion(DSStandardWrapper benchmark, int insertKeys);
	void runSearch(DSStandardWrapper benchmark, int searchKeys);
	void runRemoval(DSStandardWrapper benchmark, int removeKeys);
	void run(enum DataStructureType benchmarkType, int insertKeys, int searchKeys, int removeKeys);
	void run(enum DataStructureType benchmarkType, int insertKeys, int searchKeys, int removeKeys, int amount);
};

#endif