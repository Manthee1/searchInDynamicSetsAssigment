#ifndef RedBlackTreeWrapper_H_
#define RedBlackTreeWrapper_H_

#include "../utils.h"
#include "../../modules/RedBlackTree.h"

class RedBlackTreeWrapper : public DSStandardWrapper {
public:
	RedBlackTreeWrapper();
	~RedBlackTreeWrapper();
	void init(int) override;
	void insert(int) override;
	void search(int) override;
	void remove(int) override;
	void destroy() override;
};

#endif
