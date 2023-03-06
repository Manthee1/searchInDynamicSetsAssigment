#include "CHashTableBenchmark.h"

static CHashTable* table;
void CHashTableBenchmark::init(int capacity) {
	table = new CHashTable(capacity);
}

void CHashTableBenchmark::insert(int key) {
	table->insertKey(key, 0);
}

void CHashTableBenchmark::search(int key) {
	table->searchKey(key);
}

void CHashTableBenchmark::remove(int key) {
	table->deleteKey(key);
}

void CHashTableBenchmark::destroy() {
	delete table;
}
