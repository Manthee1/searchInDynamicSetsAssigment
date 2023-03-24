#include "OAHashTableWrapper.h"

static OAHashTable* table;

OAHashTableWrapper::OAHashTableWrapper() {
	this->name = "Open Addressing Hash Table";
}

void OAHashTableWrapper::init(int buckets) {
	table = new OAHashTable(buckets);
}

void OAHashTableWrapper::insert(int key) {
	table->insertKey(std::to_string(key), key);
}

int OAHashTableWrapper::search(int key) {
	return table->searchKey(std::to_string(key));
}
void OAHashTableWrapper::returnlessSearch(int key) {
	table->searchKey(std::to_string(key));
}

void OAHashTableWrapper::remove(int key) {
	table->deleteKey(std::to_string(key));
}

bool OAHashTableWrapper::isValid() {
	// make sure that every key has the same value as the key
	for (int i = 0; i < table->capacity; i++)
		if (table->table->keys[i] != NULL && *table->table->keys[i] != "" && table->table->values[i] != std::stoi(*table->table->keys[i]))
			return false;
	return true;
}

int OAHashTableWrapper::getSize() {
	return table->size;
}

long long OAHashTableWrapper::calculateSpaceComplexity() {
	long long spaceUsed = 0;

	// Get the size of the vectors
	spaceUsed += SIZE_VECTOR;
	spaceUsed += SIZE_VECTOR;
	// Go through each string pointer and add exsiting strings sizes to spaceUsed
	for (int i = 0; i < table->capacity; i++)
		spaceUsed += table->table->keys[i] == NULL ? 0 : (*table->table->keys[i]).size() + SIZE_STRING;	 // the size of std::string object

	// Get the size of the ints in the vector
	spaceUsed += table->capacity * (SIZE_STRING_POINTER + SIZE_INT);  // 16 is the size of std::string pinter and 4 is the size of int
	return spaceUsed;
}

void OAHashTableWrapper::destroy() {
	delete table;
}

OAHashTableWrapper::~OAHashTableWrapper() {
	delete table;
}

static int* getAllOAHashTableKeys(OAHashTable* table, int& length) {
	int* keys = new int[length];
	int index = 0;
	// Go through each bucket and vector
	for (int i = 0; i < table->capacity; i++) {
		if (table->table->keys[i] != nullptr) {
			// Convert string to int
			keys[index] = std::stoi(*table->table->keys[i]);
			index++;
		}
	}
	return keys;
}
