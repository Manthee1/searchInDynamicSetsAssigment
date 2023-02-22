#include "OpenAdressingHashTable.h"

#define DEFAULT_CAPACITY 10

void OpenAdressingHashTable::initTable(int capacity, int* keys, int* values) {
	this->capacity = capacity;
	this->size = 0;

	// If the capacity is less than the amount of keys, resize the table
	// if (capacity < (keys != nullptr ? sizeof(keys) / sizeof(keys[0]) : 0)) {
	// 	resize(sizeof(keys) / sizeof(keys[0]));
	// }

	this->table = new HashTableEntry[capacity];

	// Nullify all entries
	for (int i = 0; i < capacity; i++) table[i] = {-1, -1};
	// Insert all keys
	if (keys != nullptr && values != nullptr) {
		for (int i = 0; i < capacity; i++) {
			if (keys[i] == -1) return;
			insertKey(keys[i], values[i]);
		}
	}
}

void OpenAdressingHashTable::resize(int newCapacity) {
	// Create a new table
	HashTableEntry* newTable = new HashTableEntry[newCapacity];
	// Nullify all entries
	for (int i = 0; i < newCapacity; i++) newTable[i] = {-1, -1};
	// Insert all keys
	for (int i = 0; i < capacity; i++) {
		if (table[i].key != -1) {
			int index = hash(table[i].key);
			// If the entry is empty, create a new one
			if (newTable[index].key == -1) {
				newTable[index].key = table[i].key;
				newTable[index].value = table[i].value;
				continue;
			}

			// If the entry is not empty, probe for an empty slot
			while (newTable[index].key != -1) {
				index = (index + 1) % newCapacity;
			}
			newTable[index].key = table[i].key;
			newTable[index].value = table[i].value;
		}
	}
	// Delete the old table
	delete[] table;
	// Set the new table
	table = newTable;
	capacity = newCapacity;
}

OpenAdressingHashTable::OpenAdressingHashTable(int capacity, int* keys, int* values) {
	initTable(capacity, keys, values);
}

OpenAdressingHashTable::OpenAdressingHashTable(int capacity) {
	initTable(capacity, {}, {});
}

OpenAdressingHashTable::OpenAdressingHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {});
}

OpenAdressingHashTable::~OpenAdressingHashTable() {
}

int OpenAdressingHashTable::hash(int key) {
	// DOoble hash
	int hash1 = key % capacity;
	int hash2 = 7 - (key % 7);

	// Return the index of the key
	return (hash1 * hash2) % capacity;
}

void OpenAdressingHashTable::insertKey(int key, int value) {
	int index = hash(key);

	// Check if the table needs to be resized
	if (++size / capacity >= 0.75) resize(capacity * 2);

	// If the entry is empty, create a new one
	if (table[index].key == -1) {
		table[index].key = key;
		table[index].value = value;
		return;
	}

	// If the entry is not empty, probe for an empty slot
	while (table[index].key != -1) {
		index = (index + 1) % capacity;
	}

	table[index].key = key;
	table[index].value = value;
}

int OpenAdressingHashTable::searchKey(int key) {
	int index = getIndex(key);
	if (index == -1) return -1;
	return table[index].value;
}

int OpenAdressingHashTable::getIndex(int key) {
	int index = hash(key);
	// If the entry is empty, return -1
	if (table[index].key == -1) return -1;

	// If the entry is not empty, probe for the key
	while (table[index].key != key) {
		index = (index + 1) % capacity;
		if (table[index].key == -1) return -1;
	}
	return index;
}

void OpenAdressingHashTable::deleteKey(int key) {
	int index = getIndex(key);
	if (index == -1) return;
	table[index].key = -1;
	table[index].value = -1;
}

void OpenAdressingHashTable::clear() {
	delete[] table;
	initTable(10, nullptr, nullptr);
}
