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
	// the number of elements in the table
	int size;
	int buckets;
	float loadFactor = 0.75;

	HashTableChain* table = nullptr;

	/**
	 * @brief Construct a new CHashTable object.
	 * 		Initializes the table with the default capacity
	 */
	CHashTable();

	/**
	 * @brief Construct a new CHashTable object.
	 * 		Initializes the table with the given capacity
	 * @param buckets The number of buckets in the table
	 */
	CHashTable(int);

	/**
	 * @brief Construct a new CHashTable object.
	 * 		Initializes the table with the given capacity and inserts the given keys
	 * @param buckets The number of buckets in the table
	 * @param keys The keys to be inserted
	 * @param values The values to be inserted
	 */
	CHashTable(int, std::string*, int*);

	// Destructor
	~CHashTable();

	/**
	 * @brief Hashes the given key using the double hashing method
	 *
	 * @param key
	 * @return unsigned int
	 */
	unsigned int hash(const std::string& key);

	/**
	 * @brief Inserts the given key into the table
	 *
	 * @param key
	 * @param value
	 */
	void insertKey(std::string, int);

	/**
	 * @brief Searches for the given key in the table
	 *
	 * @param key
	 * @return int
	 */
	int searchKey(std::string);

	/**
	 * @brief Finds the index of the given key in the table if it exists
	 *
	 * @param key
	 * @return int (-1 if the key does not exist)
	 */
	int getKeyIndex(std::string, unsigned int);

	/**
	 * @brief Deletes the given key from the table
	 *
	 * @param key
	 */
	void deleteKey(std::string);

	/**
	 * @brief Clears the table
	 *
	 */
	void clear();
};

#endif