#include "OAHashTable.h"

#define DEFAULT_CAPACITY 0

void OAHashTable::initTable(int capacity, std::string* keys, int* values, int size) {
	this->capacity = capacity;
	this->size = 0;

	// If the capacity is less than the amount of keys, resize the table
	// if (capacity < (keys != nullptr ? sizeof(keys) / sizeof(keys[0]) : 0)) {
	// 	resize(sizeof(keys) / sizeof(keys[0]));
	// }
	if (capacity == 0)
		return;

	this->table = new OAHashTableData;

	// Nullify all entries
	for (int i = 0; i < capacity; i++) {
		table->keys.push_back(NULL);
		table->values.push_back(0);
	}

	// Insert all the keys and values
	for (int i = 0; i < size; i++)
		insertKey(keys[i], values[i]);
}

void OAHashTable::resize(int newCapacity) {
	// Clone the keys and values
	std::string* keys = new std::string[size];
	int* values = new int[size];

	// Go through the table and add all the keys and values to the arrays
	int index = 0;
	for (int i = 0; i < capacity; i++) {
		if (table->keys[i] == NULL)
			continue;

		keys[index] = *table->keys[i];
		values[index] = table->values[i];
		index++;
	}
	clear();
	// Delete the table
	delete table;

	// Reinitialize the table
	initTable(newCapacity, keys, values, size);

	// Delete the arrays
	delete[] keys;
	delete[] values;
}

OAHashTable::OAHashTable(int capacity, std::string* keys, int* values, int size) {
	initTable(capacity, keys, values, size);
}

OAHashTable::OAHashTable(int capacity) {
	initTable(capacity, {}, {}, 0);
}

OAHashTable::OAHashTable() {
	initTable(DEFAULT_CAPACITY, {}, {}, 0);
}

OAHashTable::~OAHashTable() {
	// Delete the keys and values
	// Delete the keys
	for (int i = 0; i < capacity; i++) {
		if (table->keys[i] == NULL) continue;
		// Delete if not null
		delete table->keys[i];
		table->keys[i] = NULL;
	}

	table->keys.clear();
	table->values.clear();
	delete table;
}

int OAHashTable::hash1(std::string key) {
	int keyValue = 0;
	for (size_t i = 0; i < key.length(); i++) keyValue += key[i];
	return keyValue % capacity;
}

int OAHashTable::hash2(std::string key) {
	int keyValue = 0;
	for (size_t i = 0; i < key.length(); i++) keyValue += key[i];
	return (hash1(key) + 1) * (keyValue % (capacity - 1));
}

void OAHashTable::insertKey(std::string key, int value) {
	int index = hash1(key);

	size++;
	// Check if the table needs to be resized
	if ((double)size / capacity >= 0.75) {
		resize(capacity * 2);
		index = hash1(key);
	}

	// If the index is empty, insert the key and value
	if (table->keys[index] == NULL) {
		table->keys[index] = new std::string(key);
		table->values[index] = value;
		return;
	}
	// Otherwise, find an empty index
	int i = 0;
	while (table->keys[index] != NULL && i++ < capacity) {
		// If the key already exists, update the value
		if (*table->keys[index] == key) {
			table->values[index] = value;

			return;
		}
		index = (index + hash2(key)) % capacity;
	}
	// If no empty index was found, resize the table and insert the key
	if (i >= capacity) {
		resize(capacity * 2);
		insertKey(key, value);
	}
}

int OAHashTable::searchKey(std::string key) {
	int index = getKeyIndex(key);
	return (index == -1) ? -1 : table->values[index];
}

int OAHashTable::getKeyIndex(std::string key) {
	int index = hash1(key);
	int i = 0;
	while (table->keys[index] != NULL && i++ < capacity) {
		// If the key is found, return the index
		if (*table->keys[index] == key) return index;
		index = (index + 1) % capacity;
	}
	return -1;
}

int OAHashTable::findEmptyIndex(int index) {
	if (table->keys[index] == NULL) return index;
	int i = 0;
	while (table->keys[index] != NULL && i++ < capacity) {
		index = (index + hash2(*table->keys[index])) % capacity;
		if (table->keys[index] == NULL)
			return index;
	}
	return -1;
}

void OAHashTable::deleteKey(std::string key) {
	int index = getKeyIndex(key);
	if (index == -1) return;
	if (table->keys[index] != NULL) {
		delete table->keys[index];
		table->keys[index] = NULL;
		size--;
	}
	table->values[index] = 0;
}

void OAHashTable::clear() {
	for (int i = 0; i < capacity; i++) {
		if (table->keys[i] != NULL) {
			delete table->keys[i];
			table->keys[i] = NULL;
		}
		table->values[i] = 0;
	}
	size = 0;
}
