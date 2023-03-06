#ifndef CHashTable_h
#define CHashTable_h
#include "../utils.h"
#include <vector>

struct HashTableChain {
	std::vector<std::string> keys;
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
	int hash(std::string key);
	void insertKey(int, int);
	void insertKey(std::string key, int value);
	int searchKey(int);
	int getKeyIndex(int, int);
	void deleteKey(int);
	void clear();
};

#endif