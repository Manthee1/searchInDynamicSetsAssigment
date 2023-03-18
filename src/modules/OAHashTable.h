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
	// the first hash function using a djb2-like hash
	int hash1(std::string);
	// The second hash function using a sdmb-like hash
	int hash2(std::string key, int iteration);
	/**
	 * @brief Resizes the table to the given capacity
	 *
	 * @param buckets The number of buckets in the table
	 */
	void resize(int);

public:
	int size;
	// the number of possible elements in the table
	int capacity;
	// the number of deleted elements that are still in the table
	int tombstoneCount;

	OAHashTableData* table;

	/**
	 * @brief Construct a new OAHashTable object.
	 *			Initializes the table with the default capacity
	 */
	OAHashTable();

	/**
	 * @brief Construct a new OAHashTable object.
	 * 			Initializes the table with the given capacity
	 * @param buckets The number of buckets in the table
	 */
	OAHashTable(int);

	/**
	 * @brief Construct a new OAHashTable object.
	 * 			Initializes the table with the given capacity and inserts the given keys
	 * @param buckets The number of buckets in the table
	 * @param keys The keys to be inserted
	 * @param values The values to be inserted
	 */
	OAHashTable(int, std::string*, int*, int);
	// Destructor
	~OAHashTable();

	/**
	 * @brief Hashes the given key using the double hashing method
	 *
	 * @param key
	 * @return unsigned int
	 */
	void insertKey(std::string, int);

	/**
	 * @brief Inserts the given key into the table
	 *
	 * @param key
	 *
	 * @return int
	 */
	int searchKey(std::string);

	/**
	 * @brief Searches for the given key in the table
	 *
	 * @param key
	 * @return int
	 */
	int getKeyIndex(std::string);

	/**
	 * @brief Returns the index of the given key in the table
	 *
	 * @param key
	 * @return int
	 */
	int getNextIndex(int index, int i, int hash2_val);

	/**
	 * @brief Returns the index of the next element in the table
	 *
	 * @param index
	 * @param i
	 * @param hash2_val
	 */
	void deleteKey(std::string);

	/**
	 * @brief Deletes the given key from the table
	 *
	 * @param key
	 */
	void clear();

	// Just runs resize() with the current capacity
	void rehash();
};

#endif