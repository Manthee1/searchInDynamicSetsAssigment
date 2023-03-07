#include "OAHashTable.h"

#define DEFAULT_CAPACITY 0

void OAHashTable::initTable(int capacity, int* keys, int* values) {
	this->capacity = capacity;
	this->size = 0;

	// If the capacity is less than the amount of keys, resize the table
	// if (capacity < (keys != nullptr ? sizeof(keys) / sizeof(keys[0]) : 0)) {
	// 	resize(sizeof(keys) / sizeof(keys[0]));
	// }
	if (capacity == 0)
		return;

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

void OAHashTable::resize(int newCapacity) {
	// Clone the keys and values
	int* keys = new int[size];
	int* values = new int[size];

	// Go through the table and add all the keys and values to the arrays
	int index = 0;
	for (int i = 0; i < capacity; i++) {
		if (table[i].key == -1)
			continue;

		keys[index] = table[i].key;
		values[index] = table[i].value;
		index++;
	}
	delete[] table;

	// Reinitialize the table
	this->table = new HashTableEntry[newCapacity];
	this->capacity = newCapacity;
	int oldSize = size;
	this->size = 0;
	// Nullify all entries
	for (int i = 0; i < capacity; i++) table[i] = {-1, -1};

	// Insert all the keys and values
	for (int i = 0; i < oldSize; i++)
		insertKey(keys[i], values[i]);

	// Delete the arrays
	delete[] keys;
	delete[] values;
}

OAHashTable::OAHashTable(int capacity, int* keys, int* values) {
	initTable(capacity, keys, values);
}

OAHashTable::OAHashTable(int capacity) {
	initTable(capacity, {}, {});
}

OAHashTable::OAHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {});
}

OAHashTable::~OAHashTable() {
	delete[] table;
}

int OAHashTable::hash(int key) {
	// DOoble hash
	int hash1 = key % capacity;
	int hash2 = 7 - (key % 7);

	// Return the index of the key
	return (hash1 * hash2) % capacity;
}

int OAHashTable::probe(int key, int i) {
	// Linear probing
	return (hash(key) + i) % capacity;
}

void OAHashTable::insertKey(int key, int value) {
	int index = hash(key);

	// Check if the table needs to be resized
	if ((double)size / capacity >= 0.75) {
		resize(capacity * 2);
		index = hash(key);
	}
	size++;

	// Probe for an empty slot
	while (table[index].key != -1) {
		// If the key already exists, update the value
		if (table[index].key == key) {
			table[index].value = value;
			return;
		}
		index = (index + 1) % capacity;
	}

	table[index].key = key;
	table[index].value = value;
}

int OAHashTable::searchKey(int key) {
	int index = getIndex(key);
	return (index == -1) ? -1 : table[index].value;
}

int OAHashTable::getIndex(int key) {
	int index = hash(key);
	int i = 0;
	while (table[index].key != -1 && table[index].key != key && i < capacity) {
		index = (index + 1) % capacity;
		i++;
	}
	return (table[index].key == key) ? index : -1;
}

void OAHashTable::deleteKey(int key) {
	int index = getIndex(key);
	if (index == -1) return;
	table[index].key = -1;
	table[index].value = -1;
}

void OAHashTable::clear() {
	delete[] table;
	initTable(10, nullptr, nullptr);
}
