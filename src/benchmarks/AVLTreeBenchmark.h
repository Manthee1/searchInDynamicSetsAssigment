#ifndef BENCHMARK_AVLTREEBENCHMARK_H_
#define BENCHMARK_AVLTREEBENCHMARK_H_

#include "../utils.h"
#include "../modules/AVLTree.h"
#include <list>

namespace AVLTreeBenchmark {
	void init();
	void insert(int key);
	void search(int key);
	void remove(int key);
	void destroy();
};

#endif
