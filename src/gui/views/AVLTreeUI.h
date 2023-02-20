#include "../../modules/AVLTree.h"

struct nodePos {
	int x;
	int y;
	Node* node;
};

namespace AVLTreeUI {
	void draw();
	void drawTreeNode(Node* node, int x, int y);
}