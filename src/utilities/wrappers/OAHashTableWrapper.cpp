#include "OAHashTableWrapper.h"

static OAHashTable* table;

OAHashTableWrapper::OAHashTableWrapper() {
	this->name = "Open Addressing Hash Table";
}

void OAHashTableWrapper::init(int buckets) {
	table = new OAHashTable(buckets);
}

void OAHashTableWrapper::insert(int key) {
	table->insertKey(std::to_string(key), key);
}

int OAHashTableWrapper::search(int key) {
	return table->searchKey(std::to_string(key));
}
void OAHashTableWrapper::returnlessSearch(int key) {
	table->searchKey(std::to_string(key));
}

void OAHashTableWrapper::remove(int key) {
	table->deleteKey(std::to_string(key));
}

bool OAHashTableWrapper::isValid() {
	return true;
}

int* OAHashTableWrapper::getAllKeys(int& length) {
	return getAllOAHashTableKeys(table, length);
}

void OAHashTableWrapper::destroy() {
	delete table;
}

OAHashTableWrapper::~OAHashTableWrapper() {
	delete table;
}

static int* getAllOAHashTableKeys(OAHashTable* table, int& length) {
	int* keys = new int[length];
	int index = 0;
	// Go through each bucket and vector
	for (int i = 0; i < table->capacity; i++) {
		if (table->table->keys[i] != nullptr) {
			// Convert string to int
			keys[index] = std::stoi(*table->table->keys[i]);
			index++;
		}
	}
	return keys;
}
