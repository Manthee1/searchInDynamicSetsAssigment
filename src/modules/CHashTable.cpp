
#include <list>
#include "CHashTable.h"

#define DEFAULT_CAPACITY 10

void CHashTable::initTable(int capacity, int* keys, int* values) {
	this->capacity = capacity;
	this->size = 0;
	this->table = new HashTableChain[capacity];
	// Nullify all entries
	for (int i = 0; i < capacity; i++) table[i] = {nullptr, nullptr, 0};
	// Insert all keys
	if (keys != nullptr && values != nullptr)
		for (int i = 0; i < capacity; i++) insertKey(keys[i], values[i]);
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
	for (int i = 0; i < capacity; i++) {
		delete[] table[i].keys;
		delete[] table[i].values;
	}
	delete[] table;
}

int CHashTable::hash(int key) {
	return key % capacity;
}

void CHashTable::resize(int newCapacity) {
}

void CHashTable::insertKey(int key, int value) {
	int index = hash(key);

	HashTableChain* entry = &table[index];

	// If the entry is empty, create a new one
	if (entry->keys == nullptr) {
		entry->keys = new int[1]{key};
		entry->values = new int[1]{value};
		entry->size = 1;
		return;
	}

	// If the entry is not empty, resize the arrays
	entry->keys = (int*)realloc(entry->keys, (entry->size + 1) * sizeof(int));
	entry->values = (int*)realloc(entry->values, (entry->size + 1) * sizeof(int));
	entry->keys[entry->size] = key;
	entry->values[entry->size] = value;
	entry->size++;
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

void CHashTable::clear() {
	for (int i = 0; i < capacity; i++) {
		delete[] table[i].keys;
		delete[] table[i].values;
		table[i].keys = nullptr;
		table[i].values = nullptr;
		table[i].size = 0;
	}
}
