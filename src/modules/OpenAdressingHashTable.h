#ifndef OpenAdressingHashTable_h
#define OpenAdressingHashTable_h
#include "../utils.h"

struct HashTableEntry {
	int key;
	int value;
};

class OpenAdressingHashTable {
private:
	void initTable(int, int*, int*);
	void resize(int);

public:
	int size;
	int capacity;
	HashTableEntry* table;
	OpenAdressingHashTable();
	OpenAdressingHashTable(int);
	OpenAdressingHashTable(int, int*, int*);
	~OpenAdressingHashTable();
	int hash(int);
	void insertKey(int, int);
	int searchKey(int);
	int getIndex(int);
	void deleteKey(int);
	void clear();
};

#endif