#ifndef ChainingHashTable_h
#define ChainingHashTable_h
#include "../utils.h"

struct HashTableChain {
	int* keys;
	int* values;
	int size;
};

class ChainingHashTable {
private:
	void initTable(int, int*, int*);
	void resize(int);
	int hash1(int);
	int hash2(int);

public:
	int size;
	int capacity;
	HashTableChain* table;
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