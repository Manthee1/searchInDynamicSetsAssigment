#ifndef AVLTreeWrapper_H_
#define AVLTreeWrapper_H_

#include "../utils.h"
#include "../modules/AVLTree.h"

static int* getAllAVLTreeKeys(AVLTree* tree, AVLNode* node, int& length);
static bool isAVLTreeBalanced(AVLTree* tree, AVLNode* node);

class AVLTreeWrapper : public DSStandardWrapper {
public:
	AVLTreeWrapper();
	~AVLTreeWrapper();
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
