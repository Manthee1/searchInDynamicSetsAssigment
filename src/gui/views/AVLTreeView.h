#include "../../modules/AVLTree.h"

struct nodePos {
	int x;
	int y;
	AVLNode* node;
};

namespace AVLTreeView {
	void draw();
	void drawTreeNode(AVLNode* node, int x, int y, int level);
}