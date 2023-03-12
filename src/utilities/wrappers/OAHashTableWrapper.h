#ifndef OAHashTableWrapper_H_
#define OAHashTableWrapper_H_

#include "../utils.h"
#include "../../modules/OAHashTable.h"

static int* getAllOAHashTableKeys(OAHashTable* table, int& length);

class OAHashTableWrapper : public DSStandardWrapper {
public:
	OAHashTableWrapper();
	~OAHashTableWrapper();
	void init(int) override;
	void insert(int) override;
	int search(int) override;
	void returnlessSearch(int) override;
	void remove(int) override;
	bool isValid() override;
	int* getAllKeys(int& length) override;
	void destroy() override;
};

#endif
