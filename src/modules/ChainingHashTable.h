#ifndef ChainingHashTable_h
#define ChainingHashTable_h
#include "../utils.h"
#include <list>

struct HashTableEntry {
	int key;
	int value;
};

class ChainingHashTable {
private:
	int size;
	int capacity;
	std::list<HashTableEntry> *table;
	void initTable(int, std::initializer_list<HashTableEntry>);
	int hash(int);
	void resize(int);

public:
	ChainingHashTable();
	ChainingHashTable(int);
	ChainingHashTable(int, std::initializer_list<HashTableEntry>);
	~ChainingHashTable();
	void insertKey(int, int);
	int searchKey(int);
	void deleteKey(int);
};

#endif