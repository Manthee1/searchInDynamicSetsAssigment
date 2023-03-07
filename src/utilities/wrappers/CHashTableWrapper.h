#ifndef CHashTableWrapper_H_
#define CHashTableWrapper_H_

#include "../utils.h"
#include "../modules/CHashTable.h"

class CHashTableWrapper : public DSStandardWrapper {
public:
	CHashTableWrapper();
	~CHashTableWrapper();
	void init(int) override;
	void insert(int) override;
	void search(int) override;
	void remove(int) override;
	void destroy() override;
};

#endif
