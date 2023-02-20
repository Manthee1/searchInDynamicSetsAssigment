
#include <list>
#include "ChainingHashTable.h"

#define DEFAULT_CAPACITY 10

void ChainingHashTable::initTable(int capacity, int* keys, int* values) {
	this->capacity = capacity;
	this->size = 0;
	this->table = new HashTableEntry[capacity];
	// Nullify all entries
	for (int i = 0; i < capacity; i++) table[i] = {nullptr, nullptr, 0};
	// Insert all keys
	for (int i = 0; i < capacity; i++) insertKey(keys[i], values[i]);
}

ChainingHashTable::ChainingHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {});
}

ChainingHashTable::ChainingHashTable(int capacity) {
	initTable(capacity, {}, {});
}

ChainingHashTable::ChainingHashTable(int capacity, int* keys, int* values) {
	initTable(capacity, keys, values);
}

ChainingHashTable::~ChainingHashTable() {
	delete[] table;
}

int ChainingHashTable::hash(int key) {
	return key % capacity;
}

void ChainingHashTable::resize(int newCapacity) {
}

void ChainingHashTable::insertKey(int key, int value) {
	int index = hash(key);
	// If the entry is empty, create a new one
	if (table[index].keys == nullptr) {
		table[index].keys = new int[1];
		table[index].values = new int[1];
		table[index].size = 1;
		table[index].keys[0] = key;
		table[index].values[0] = value;
		return;
	}

	// If the entry is not empty, resize the arrays
	table[index].keys = (int*)realloc(table[index].keys, (table[index].size + 1) * sizeof(int));
	table[index].values = (int*)realloc(table[index].values, (table[index].size + 1) * sizeof(int));
	table[index].keys[table[index].size] = key;
	table[index].values[table[index].size] = value;
	table[index].size++;
}

int ChainingHashTable::searchKey(int key) {
	int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return -1;
	return table[index].values[keyIndex];
}

int ChainingHashTable::getKeyIndex(int key, int index) {
	for (int i = 0; i < table[index].size; i++)
		if (table[index].keys[i] == key) return i;
	return -1;
}

void ChainingHashTable::deleteKey(int key) {
	int index = hash(key);
	int keyIndex = getKeyIndex(key, index);
	if (keyIndex == -1) return;

	// If the entry has only one key, delete the entry
	if (table[index].size == 1) {
		delete[] table[index].keys;
		delete[] table[index].values;
		table[index].keys = nullptr;
		table[index].values = nullptr;
		table[index].size = 0;
		return;
	}

	// If the entry has more than one key, resize the arrays
	table[index].keys = (int*)realloc(table[index].keys, (table[index].size - 1) * sizeof(int));
	table[index].values = (int*)realloc(table[index].values, (table[index].size - 1) * sizeof(int));
	table[index].size--;
	for (int i = keyIndex; i < table[index].size; i++) {
		table[index].keys[i] = table[index].keys[i + 1];
		table[index].values[i] = table[index].values[i + 1];
	}
}

void ChainingHashTable::clear() {
	for (int i = 0; i < capacity; i++) {
		delete[] table[i].keys;
		delete[] table[i].values;
		table[i].keys = nullptr;
		table[i].values = nullptr;
		table[i].size = 0;
	}
}
