#ifndef CHTABLEBENCHMARK_H_
#define CHTABLEBENCHMARK_H_

#include "../utils.h"
#include "../modules/CHashTable.h"

namespace CHashTableBenchmark {
	void init(int capacity);
	void insert(int key);
	void search(int key);
	void remove(int key);
	void destroy();
};

#endif
