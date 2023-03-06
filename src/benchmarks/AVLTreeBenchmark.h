#ifndef BENCHMARK_AVLTREEBENCHMARK_H_
#define BENCHMARK_AVLTREEBENCHMARK_H_

#include "../utils.h"
#include "../modules/AVLTree.h"

namespace AVLTreeBenchmark {
	void init(int);
	void insert(int key);
	void search(int key);
	void remove(int key);
	void destroy();
};

#endif
