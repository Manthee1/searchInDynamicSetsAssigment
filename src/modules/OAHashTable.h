#ifndef OAHashTable_h
#define OAHashTable_h
#include "../utils.h"

struct HashTableEntry {
	int key;
	int value;
};

class OAHashTable {
private:
	void initTable(int, int*, int*);
	void resize(int);

public:
	int size;
	int capacity;
	HashTableEntry* table;
	OAHashTable();
	OAHashTable(int);
	OAHashTable(int, int*, int*);
	~OAHashTable();
	int hash(int);
	void insertKey(int, int);
	int searchKey(int);
	int getIndex(int);
	void deleteKey(int);
	void clear();
};

#endif