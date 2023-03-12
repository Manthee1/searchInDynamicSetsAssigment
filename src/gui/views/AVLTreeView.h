#include "../../modules/AVLTree.h"

struct AVLNodePos {
	AVLNode* node;
	int x;
	int y;
};

namespace AVLTreeView {
	void draw();
	void drawTreeNode(AVLNode* node, int x, int y, int level);
}