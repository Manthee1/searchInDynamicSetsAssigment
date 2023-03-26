#include "CHashTableWrapper.h"

static CHashTable* table;

CHashTableWrapper::CHashTableWrapper() {
	this->name = "Chaining Hash Table";
}

void CHashTableWrapper::init(int buckets) {
	table = new CHashTable(buckets);
}

void CHashTableWrapper::insert(int key) {
	table->insertKey(std::to_string(key), key);
}

int CHashTableWrapper::search(int key) {
	return table->searchKey(std::to_string(key));
}
void CHashTableWrapper::returnlessSearch(int key) {
	table->searchKey(std::to_string(key));
}

void CHashTableWrapper::remove(int key) {
	table->deleteKey(std::to_string(key));
}

bool CHashTableWrapper::isValid() {
	// make sure that every key has the same value as the key
	for (int i = 0; i < table->buckets; i++)
		for (int j = 0; j < table->table[i].size; j++)
			if (table->table[i].values[j] != std::stoi(table->table[i].keys[j]))
				return false;

	return true;
}

int CHashTableWrapper::getSize() {
	return table->size;
}

long long CHashTableWrapper::calculateSpaceComplexity() {
	long long spaceUsed = 0;

	// Get te size of each key in the vector and the size of the vector
	for (int i = 0; i < table->buckets; i++) {
		for (int j = 0; j < table->table[i].size; j++)
			spaceUsed += table->table[i].keys[j].size() + SIZE_STRING;	// 32 is the size of std::string object
		// Get the size of the ints in the vector
		spaceUsed += table->table[i].size * SIZE_INT;
	}
	// Get the size of the keys nad values vectors and the int size variable
	spaceUsed += sizeof(table->buckets) + table->buckets * (SIZE_VECTOR + SIZE_VECTOR + SIZE_INT);
	return spaceUsed;
}

void CHashTableWrapper::destroy() {
	delete table;
}

CHashTableWrapper::~CHashTableWrapper() {
	delete table;
}
