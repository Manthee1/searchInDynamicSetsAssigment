
#include <list>
#include <vector>
#include "CHashTable.h"

#define DEFAULT_CAPACITY 10

void CHashTable::initTable(int capacity, int* keys, int* values) {
	this->capacity = capacity;
	this->size = 0;
	this->table = new HashTableChain[capacity];
	// Nullify all entries
	// for (int i = 0; i < capacity; i++) table[i] = {{}, {}, 0};
	// Insert all keys
	// if (keys != nullptr && values != nullptr)
	// 	for (int i = 0; i < capacity; i++) insertKey(keys[i], values[i]);
}

CHashTable::CHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {});
}

CHashTable::CHashTable(int capacity) {
	initTable(capacity, {}, {});
}

CHashTable::CHashTable(int capacity, int* keys, int* values) {
	initTable(capacity, keys, values);
}

CHashTable::~CHashTable() {
	delete[] table;
}

int CHashTable::hash(int key) {
	return key % capacity;
}

void CHashTable::insertKey(int key, int value) {
	int index = hash(key);

	// If the entry is empty, create a new entry
	table[index].keys.push_back(key);
	table[index].values.push_back(value);
	table[index].size++;
	size++;
}

int CHashTable::searchKey(int key) {
	int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return -1;
	return table[index].values[keyIndex];
}

int CHashTable::getKeyIndex(int key, int index) {
	for (int i = 0; i < table[index].size; i++)
		if (table[index].keys[i] == key) return i;
	return -1;
}

void CHashTable::deleteKey(int key) {
	int index = hash(key);
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
	for (int i = 0; i < capacity; i++) {
		table[i].keys.clear();
		table[i].values.clear();
		table[i].size = 0;
	}
}
