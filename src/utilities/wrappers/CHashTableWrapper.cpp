#include "CHashTableWrapper.h"

static CHashTable* table;

CHashTableWrapper::CHashTableWrapper() {
	this->name = "Chaining Hash Table";
}

void CHashTableWrapper::init(int capacity) {
	table = new CHashTable(capacity);
}

void CHashTableWrapper::insert(int key) {
	table->insertKey(key, 0);
}

void CHashTableWrapper::search(int key) {
	table->searchKey(key);
}

void CHashTableWrapper::remove(int key) {
	table->deleteKey(key);
}

void CHashTableWrapper::destroy() {
	delete table;
}

CHashTableWrapper::~CHashTableWrapper() {
	delete table;
}
