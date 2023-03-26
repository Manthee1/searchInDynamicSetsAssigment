#include "OAHashTable.h"

#define DEFAULT_CAPACITY 0

static bool isKeyValid(std::string key) {
	return key != "";
}
static bool isValueValid(int value) {
	return value != -1;
}

void OAHashTable::initTable(int capacity, std::string* keys, int* values, int size) {
	this->capacity = capacity;
	this->size = 0;
	this->tombstoneCount = 0;

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
	// Cap the min capacity to 5
	newCapacity = max(newCapacity, 5);

	// Clone the keys and values
	std::string* keys = new std::string[size];
	int* values = new int[size];

	// Go through the table and add all the keys and values to the arrays
	int index = 0;
	for (int i = 0; i < capacity; i++) {
		if (table->keys[i] == NULL)
			continue;
		// If the key is a thousand years old, it's dead, Jim. Kidding. But really if the key is a tombstone, delete it and continue
		if (*table->keys[i] == "") {
			delete table->keys[i];
			table->keys[i] = NULL;
			table->values[i] = 0;
			this->tombstoneCount--;
			continue;
		}
		keys[index] = *table->keys[i];
		values[index] = table->values[i];
		index++;
	}

	// Delete the table and reinitialize it
	clear();
	capacity = newCapacity;

	// Clear the keys and values because clear() just sets them to null
	table->keys.clear();
	table->values.clear();

	// Reinitialize the table
	for (int i = 0; i < capacity; i++) {
		table->keys.push_back(NULL);
		table->values.push_back(0);
	}

	// Insert all the keys and values
	for (int i = 0; i < index; i++)
		insertKey(keys[i], values[i]);

	// Delete the arrays
	delete[] keys;
	delete[] values;
}

void OAHashTable::rehash() {
	// Just run resize with the same capacity. because I'm lazy
	resize(capacity);
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
		tombstoneCount--;
	}

	table->keys.clear();
	table->values.clear();
	delete table;
}

int OAHashTable::hash1(std::string key) {
	// djb2-like hash function
	unsigned long hash = 5381;
	for (size_t i = 0; i < key.length(); i++)
		hash = ((hash << 5) + hash) + key[i];
	return hash % capacity;
}

int OAHashTable::hash2(std::string key, int iteration) {
	// sdbm-like hash function
	unsigned int hash = 0;
	for (size_t i = 0; i < key.length(); i++)
		hash = key[i] + (hash << 6) + (hash << 16) - hash;
	return (hash + iteration) % (capacity - 1) + 1;
}

void OAHashTable::insertKey(std::string key, int value) {
	if (!isKeyValid(key) || !isValueValid(value)) return;
	int index = hash1(key);
	int hash2_val = hash2(key, 0);
	int i = 1;

	// Check if the table needs to be resized
	if ((double)size / capacity >= 0.75) {
		resize(capacity * 2);
		index = hash1(key);
		hash2_val = hash2(key, 0);
	}

	// Probe the table using double hashing until an empty slot is found
	while (table->keys[index] != NULL) {
		// If the key already exists, update its value and return
		if (*table->keys[index] == key) {
			table->values[index] = value;
			return;
		}
		index = getNextIndex(index, i, hash2_val);
		i++;
	}

	// Insert the key and value into the first available slot
	table->keys[index] = new std::string(key);
	table->values[index] = value;
	size++;
}

int OAHashTable::searchKey(std::string key) {
	if (!isKeyValid(key)) return -1;
	int index = getKeyIndex(key);
	return (index == -1) ? -1 : table->values[index];
}

int OAHashTable::getNextIndex(int index, int i, int hash2_val) {
	// If i is greater than 10, use linear probing otherwise use double hashing
	return (i > 10) ? (index + 1) % capacity : (index + i * hash2_val) % capacity;
}

int OAHashTable::getKeyIndex(std::string key) {
	int index = hash1(key);
	int hash2_val = hash2(key, 0);
	int i = 0;
	// Make sure we try linear search that activates after 10 failed double hash attempts
	while (table->keys[index] != NULL && i < 11 + capacity) {
		// If the key is found, return the index
		if (*table->keys[index] == key) return index;
		index = getNextIndex(index, i, hash2_val);
		i++;
	}
	return -1;
}

void OAHashTable::deleteKey(std::string key) {
	if (!isKeyValid(key)) return;
	int index = getKeyIndex(key);
	if (index == -1) return;
	if (table->keys[index] == NULL) return;
	delete table->keys[index];
	// Set the key to a tombstone
	table->keys[index] = new std::string("");
	size--;
	table->values[index] = 0;

	tombstoneCount++;

	// If the table is less than 25% full, resize it
	if ((double)size / capacity <= 0.25) resize(capacity / 2);

	// If the table has more than 50% tombstones, rehash it
	if ((double)tombstoneCount / capacity >= 0.5) {
		rehash();
		tombstoneCount = 0;
	}
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
