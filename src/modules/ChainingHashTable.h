#ifndef ChainingHashTable_h
#define ChainingHashTable_h
#include "../utils.h"
#include <list>

struct HashTableEntry {
	int* keys;
	int* values;
	int size;
};

class ChainingHashTable {
private:
	void initTable(int, int*, int*);
	void resize(int);

public:
	int size;
	int capacity;
	HashTableEntry* table;
	ChainingHashTable();
	ChainingHashTable(int);
	ChainingHashTable(int, int*, int*);
	~ChainingHashTable();
	int hash(int);
	void insertKey(int, int);
	int searchKey(int);
	int getKeyIndex(int, int);
	void deleteKey(int);
	void clear();
};

#endif