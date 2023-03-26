#ifndef RedBlackTreeWrapper_H_
#define RedBlackTreeWrapper_H_

#include "../utils.h"
#include "../../modules/RedBlackTree.h"

static bool isRedBlackTreeValid(RedBlackTree* tree);

class RedBlackTreeWrapper : public DSStandardWrapper {
public:
	RedBlackTreeWrapper();
	~RedBlackTreeWrapper();
	void init(int) override;
	void insert(int) override;
	int search(int) override;
	void returnlessSearch(int) override;
	void remove(int) override;
	bool isValid() override;
	int getSize() override;
	long long calculateSpaceComplexity() override;
	void destroy() override;
};

#endif
