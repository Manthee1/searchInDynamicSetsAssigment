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

int* CHashTableWrapper::getAllKeys(int& length) {
	return getAllCHashTableKeys(table, length);
}

int CHashTableWrapper::calculateSpaceComplexity() {
	int spaceUsed = 0;

	for (int i = 0; i < table->buckets; i++) {
		for (int j = 0; j < table->table[i].size; j++) {
			spaceUsed += table->table[i].keys[j].size() + SIZE_STRING;	// 32 is the size of std::string object
		}
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

static int* getAllCHashTableKeys(CHashTable* table, int& length) {
	int* keys = new int[length];
	int index = 0;
	// Go through each bucket and vector
	for (int i = 0; i < table->buckets; i++) {
		for (int j = 0; j < table->table[i].size; j++) {
			// Convert string to int
			keys[index] = std::stoi(table->table[i].keys[j]);
			index++;
		}
	}
	return keys;
}
