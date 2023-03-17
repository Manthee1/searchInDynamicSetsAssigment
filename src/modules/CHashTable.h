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
	void initTable(int, std::string*, int*);
	void resize(int);

public:
	int size;
	int buckets;
	float loadFactor = 0.75;
	HashTableChain* table = nullptr;
	CHashTable();
	CHashTable(int);
	CHashTable(int, std::string*, int*);
	~CHashTable();
	unsigned int hash(const std::string& key);
	void insertKey(std::string, int);
	int searchKey(std::string);
	int getKeyIndex(std::string, unsigned int);
	void deleteKey(std::string);
	void clear();
};

#endif