#ifndef OAHashTable_h
#define OAHashTable_h
#include "../utils.h"
#include <vector>

struct OAHashTableData {
	// vector of string pointers
	std::vector<std::string*> keys;
	std::vector<int> values;
};

class OAHashTable {
private:
	void initTable(int, std::string*, int*, int);
	int hash1(std::string);
	int hash2(std::string);
	void resize(int);

public:
	int size;
	int capacity;
	OAHashTableData* table;
	OAHashTable();
	OAHashTable(int);
	OAHashTable(int, std::string*, int*, int);
	~OAHashTable();

	void insertKey(std::string, int);
	int searchKey(std::string);
	int getKeyIndex(std::string);
	int findEmptyIndex(int index);
	void deleteKey(std::string);
	void clear();
};

#endif