#ifndef CHashTableWrapper_H_
#define CHashTableWrapper_H_

#include "../utils.h"
#include "../modules/CHashTable.h"

static int* getAllCHashTableKeys(CHashTable* table, int& length);

class CHashTableWrapper : public DSStandardWrapper {
public:
	CHashTableWrapper();
	~CHashTableWrapper();
	void init(int) override;
	void insert(int) override;
	int search(int) override;
	void returnlessSearch(int) override;
	void remove(int) override;
	bool isValid() override;
	int* getAllKeys(int& length) override;
	int calculateSpaceComplexity() override;
	void destroy() override;
};

#endif
