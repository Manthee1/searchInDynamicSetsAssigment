#ifndef AVLTreeWrapper_H_
#define AVLTreeWrapper_H_

#include "../utils.h"
#include "../modules/AVLTree.h"

class AVLTreeWrapper : public DSStandardWrapper {
public:
	AVLTreeWrapper();
	~AVLTreeWrapper();
	void init(int) override;
	void insert(int) override;
	void search(int) override;
	void remove(int) override;
	void destroy() override;
};

#endif
