#include "../../modules/RedBlackTree.h"

struct RBnodePos {
	RedBlackNode* node;
	int x;
	int y;
};

namespace RedBlackTreeView {
	void draw();
	void drawTreeNode(RedBlackNode* node, int x, int y, int level);
}