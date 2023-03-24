
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

void CHashTable::resize(int newBuckets) {
	// Cap the min buckets to 5
	newBuckets = max(newBuckets, 5);

	// Create a new table
	HashTableChain* newTable = new HashTableChain[newBuckets];
	int oldBuckets = buckets;
	buckets = newBuckets;
	// Nullify all entries
	for (int i = 0; i < buckets; i++) newTable[i] = {{}, {}, 0};
	// Insert all keys
	for (int i = 0; i < oldBuckets; i++) {
		for (int j = 0; j < table[i].size; j++) {
			std::string key = table[i].keys[j];
			int value = table[i].values[j];
			unsigned int index = hash(key);
			newTable[index].keys.push_back(key);
			newTable[index].values.push_back(value);
			newTable[index].size++;
		}
	}
	// Delete the old table
	delete[] table;
	// Set the new table
	table = newTable;
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
	// Check if the table is initialized
	if (table == nullptr) return;
	// Delete the table
	for (int i = 0; i < buckets; i++) {
		table[i].keys.clear();
		table[i].values.clear();
		table[i].size = 0;
	}
	delete[] table;
}

unsigned int CHashTable::hash(const std::string& key) {
	unsigned long long hash1 = 0, hash2 = 0;
	for (size_t i = 0; i < key.length(); i++) {
		hash1 = (hash1 << 4) + key[i];
		hash2 = (hash2 << 5) + key[i];
	}
	return (hash1 % buckets + hash2 % (buckets - 1) + 1) % buckets;
}

void CHashTable::insertKey(std::string key, int value) {
	unsigned int index = hash(key);
	// Find if key already exists
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex != -1) {
		table[index].values[keyIndex] = value;
		return;
	}

	table[index].keys.push_back(key);
	table[index].values.push_back(value);
	table[index].size++;
	size++;

	if (size >= buckets * loadFactor) resize(buckets * 2);
}

int CHashTable::searchKey(std::string key) {
	unsigned int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return -1;
	return table[index].values[keyIndex];
}

int CHashTable::getKeyIndex(std::string key, unsigned int index) {
	// Find if key already exists
	for (int i = 0; i < table[index].size; i++)
		if (table[index].keys[i] == key) return i;
	return -1;
}

void CHashTable::deleteKey(std::string key) {
	unsigned int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return;

	// // If the entry has only one key, delete the entry
	// if (table[index].size == 1) {
	// 	table[index].keys.clear();
	// 	table[index].values.clear();
	// 	table[index].size = 0;
	// 	size--;
	// 	if (size <= buckets * loadFactor / 2) resize(buckets / 2);
	// 	return;
	// }

	// If the entry has more than one key, resize the arrays
	table[index].keys.erase(table[index].keys.begin() + keyIndex);
	table[index].values.erase(table[index].values.begin() + keyIndex);
	table[index].size--;
	size--;
	// Resize the table if the load factor is too low
	if (size <= buckets * loadFactor / 2) resize(buckets / 2);
}

void CHashTable::clear() {
	for (int i = 0; i < buckets; i++) {
		table[i].keys.clear();
		table[i].values.clear();
		table[i].size = 0;
	}
	size = 0;
}
