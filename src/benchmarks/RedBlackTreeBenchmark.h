#ifndef BENCHMARK_REDBBLACKTREEBENCHMARK_H_
#define BENCHMARK_REDBBLACKTREEBENCHMARK_H_

#include "../utils.h"
#include "../modules/RedBlackTree.h"

namespace RedBlackTreeBenchmark {
	void init(int);
	void insert(int key);
	void search(int key);
	void remove(int key);
	void destroy();
};

#endif
