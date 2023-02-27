#include "RedBlackTreeView.h"
#include <math.h>
#include <sstream>
#include <gui.h>
#include <cstdlib>
#define IG GUI::imgui

static RedBlackTree tree;
static int insertKey = 0;
static int removeKey = 0;

void RedBlackTreeView::draw() {
	IG::Begin("Add RedBlackNode");
	IG::InputInt("Key", &insertKey);

	if (IG::Button("Add RedBlackNode")) {
		tree.insertKey(insertKey);
	}
	IG::End();

	GUI::beginMain();
	drawTreeNode(tree.root, IG::GetIO().DisplaySize.x / 2, 50, 0);
	IG::End();
}

void RedBlackTreeView::drawTreeNode(RedBlackNode* node, int x, int y, int level) {
	if (node == NULL) return;

	// Draw the node
	GUI::circle(x, y, 20, new int[3]{node->color == red ? 255 : 0, 0, 0});
	GUI::text(x - 10, y - 10, std::to_string(node->key), new int[3]{255, 255, 255});

	int xeq = 100 * pow(2, level);
	if (node->left != NULL) {
		//  Draw a line from the current node to the left node factoring in the height of the tree
		// If the line is not on the screen, don't draw it
		if (GUI::isOnScreen(x - xeq, y + 100) || GUI::isOnScreen(x, y))
			GUI::line(x, y, x - xeq, y + 100, new int[3]{255, 255, 255});
		// Draw the left node
		drawTreeNode(node->left, x - xeq, y + 100, level - 1);
	}
	if (node->right != NULL) {
		// Draw a line from the current node to the right node factoring in the height of the tree
		// If the line is not on the screen, don't draw it
		if (GUI::isOnScreen(x + xeq, y + 100) || GUI::isOnScreen(x, y))
			GUI::line(x, y, x + xeq, y + 100, new int[3]{255, 255, 255});
		// Draw the right node
		drawTreeNode(node->right, x + xeq, y + 100, level - 1);
	}
}
