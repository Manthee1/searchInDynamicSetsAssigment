
#include <list>
#include "ChainingHashTable.h"

#define DEFAULT_CAPACITY 10

void ChainingHashTable::initTable(int capacity, std::initializer_list<HashTableEntry> list) {
	this->capacity = capacity;
	this->size = 0;
	table = new std::list<HashTableEntry>[capacity];
}

ChainingHashTable::ChainingHashTable() {
	initTable(DEFAULT_CAPACITY, {});
}

ChainingHashTable::ChainingHashTable(int capacity) {
	initTable(capacity, {});
}

ChainingHashTable::ChainingHashTable(int capacity, std::initializer_list<HashTableEntry> list) {
	initTable(capacity, list);
}

ChainingHashTable::~ChainingHashTable() {
	delete[] table;
}

int ChainingHashTable::hash(int key) {
	return key % capacity;
}

void ChainingHashTable::resize(int newSize) {
}

void ChainingHashTable::insertKey(int key, int value) {
}

int ChainingHashTable::searchKey(int key) {
}

void ChainingHashTable::deleteKey(int key) {
}
