#ifndef CHashTable_h
#define CHashTable_h
#include "../utils.h"
#include <vector>

struct HashTableChain {
	std::vector<int> keys;
	std::vector<int> values;
	int size;
};

class CHashTable {
private:
	void initTable(int, int*, int*);

public:
	int size;
	int capacity;
	HashTableChain* table;
	CHashTable();
	CHashTable(int);
	CHashTable(int, int*, int*);
	~CHashTable();
	int hash(int);
	void insertKey(int, int);
	int searchKey(int);
	int getKeyIndex(int, int);
	void deleteKey(int);
	void clear();
};

#endif