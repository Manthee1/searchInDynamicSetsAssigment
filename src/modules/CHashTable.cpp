
#include <list>
#include <vector>
#include "CHashTable.h"

#define DEFAULT_CAPACITY 0
#define LOAD_FACTOR 0.75

void CHashTable::initTable(int buckets, std::string* keys, int* values) {
	this->buckets = buckets;
	this->size = 0;
	if (buckets == 0) return;
	this->table = new HashTableChain[buckets];
	// Nullify all entries
	for (int i = 0; i < buckets; i++) table[i] = {{}, {}, 0};
	// Insert all keys
	if (keys != nullptr && values != nullptr)
		for (int i = 0; i < buckets; i++) insertKey(keys[i], values[i]);
}

CHashTable::CHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {});
}

CHashTable::CHashTable(int buckets) {
	initTable(buckets, {}, {});
}

CHashTable::CHashTable(int buckets, std::string* keys, int* values) {
	initTable(buckets, keys, values);
}

CHashTable::~CHashTable() {
	for (int i = 0; i < buckets; i++) {
		// std::cout << "Deleting bucket " << i << " :";
		// for (int j = 0; j < table[i].size; j++) {
		// 	std::cout << table[i].keys[j];
		// }
		// std::cout << std::endl;
		table[i].keys.clear();
		table[i].values.clear();
	}
	delete[] table;
}

unsigned int CHashTable::hash(const std::string& key) {
	const unsigned int factor = 31;	 // a prime number
	unsigned long hash = 0;
	for (size_t i = 0; i < key.length(); i++)
		hash = (hash * factor) + key[i];

	return hash % buckets;
}

void CHashTable::insertKey(std::string key, int value) {
	unsigned int index = hash(key);
	table[index].keys.push_back(key);
	table[index].values.push_back(value);
	table[index].size++;
	size++;
}

int CHashTable::searchKey(std::string key) {
	unsigned int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return -1;
	return table[index].values[keyIndex];
}

int CHashTable::getKeyIndex(std::string key, unsigned int index) {
	for (int i = 0; i < table[index].size; i++)
		if (table[index].keys[i] == key) return i;
	return -1;
}

void CHashTable::deleteKey(std::string key) {
	unsigned int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return;

	// If the entry has only one key, delete the entry
	if (table[index].size == 1) {
		table[index].keys.clear();
		table[index].values.clear();
		table[index].size = 0;
		size--;
		return;
	}

	// If the entry has more than one key, resize the arrays
	table[index].keys.erase(table[index].keys.begin() + keyIndex);
	table[index].values.erase(table[index].values.begin() + keyIndex);
	table[index].size--;
	size--;
}

void CHashTable::clear() {
	for (int i = 0; i < buckets; i++) {
		table[i].keys.clear();
		table[i].values.clear();
		table[i].size = 0;
	}
	size = 0;
}
