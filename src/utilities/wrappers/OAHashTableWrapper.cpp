#include "OAHashTableWrapper.h"

static OAHashTable* table;

OAHashTableWrapper::OAHashTableWrapper() {
	this->name = "Open Addressing Hash Table";
}

void OAHashTableWrapper::init(int capacity) {
	table = new OAHashTable(capacity);
}

void OAHashTableWrapper::insert(int key) {
	table->insertKey(key, 0);
}

void OAHashTableWrapper::search(int key) {
	table->searchKey(key);
}

void OAHashTableWrapper::remove(int key) {
	table->deleteKey(key);
}

void OAHashTableWrapper::destroy() {
	delete table;
}

OAHashTableWrapper::~OAHashTableWrapper() {
	delete table;
}
