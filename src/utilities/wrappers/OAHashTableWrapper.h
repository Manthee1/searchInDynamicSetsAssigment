#ifndef OAHashTableWrapper_H_
#define OAHashTableWrapper_H_

#include "../utils.h"
#include "../../modules/OAHashTable.h"

class OAHashTableWrapper : public DSStandardWrapper {
public:
	OAHashTableWrapper();
	~OAHashTableWrapper();
	void init(int) override;
	void insert(int) override;
	void search(int) override;
	void remove(int) override;
	void destroy() override;
};

#endif
