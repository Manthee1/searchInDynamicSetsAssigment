#include "AVLTreeView.h"
#include <math.h>
#include <sstream>
#include <gui.h>
#include <cstdlib>
#define IG GUI::imgui

static int insertKey = 0;
static int removeKey = 0;
nodePos** nodePositions = NULL;
static int nodePositionsSize = 0;
static bool reIndexNodes = true;

void resetNodeGUIData() {
	// Reset the node positions
	for (int i = 0; i < nodePositionsSize; i++) {
		delete nodePositions[i];
	}
	delete nodePositions;
	nodePositionsSize = 0;
	nodePositions = NULL;
	reIndexNodes = true;
}

AVLTree tree;
void AVLTreeView::draw() {
	// Add a node to the tree
	IG::Begin("Add Node");
	IG::InputInt("Key", &insertKey);

	if (IG::Button("Add Node")) {
		tree.insertKey(insertKey);
		resetNodeGUIData();
	}
	IG::End();

	// Remove a node from the tree
	IG::Begin("Remove Node");
	IG::InputInt("Key", &removeKey);
	if (IG::Button("Remove Node")) {
		tree.deleteKey(removeKey);
	}
	if (IG::Button("Clear Tree")) {
		delete tree.root;
		tree.root = NULL;
		resetNodeGUIData();
	}
	IG::End();

	IG::Begin("Create Random Tree");
	static int size = 10;
	IG::InputInt("Amount", &size);
	if (IG::Button("Create Table")) {
		delete tree.root;
		tree.root = NULL;
		for (int i = 0; i < size; i++)
			tree.insertKey(rand() % 100);
		resetNodeGUIData();
	}
	IG::End();

	// Check if a node was clicked
	if (IG::IsMouseClicked(0)) {
		int x = IG::GetMousePos().x;
		int y = IG::GetMousePos().y;
		std::cout << "Mouse clicked at (" << x << ", " << y << ")" << std::endl;

		// Check if the mouse was clicked on a node
		for (int i = 0; i < nodePositionsSize; i++) {
			nodePos* pos = nodePositions[i];
			// use sqrt to calculate the distance between the mouse and the node
			if (sqrt(pow(pos->x - x, 2) + pow(pos->y - y, 2)) < 20) {
				// The mouse was clicked on a node
				std::cout << "Clicked on node with key " << pos->node->key << std::endl;
				tree.deleteNode(pos->node);
				resetNodeGUIData();
				break;
			}
		}
	}

	// Draw the tree
	GUI::beginMain();
	drawTreeNode(tree.root, IG::GetIO().DisplaySize.x / 2, 50);
	IG::End();
	reIndexNodes = false;
}

void AVLTreeView::drawTreeNode(Node* node, int x, int y) {
	if (node == NULL) return;
	// Draw the tree
	// Draw a circle at the current node
	GUI::circle(x, y, 20, new int[3]{255, 0, 0});
	// Draw the key of the current node
	GUI::text(x - 10, y - 10, std::to_string(node->key), new int[3]{255, 255, 255});

	int xeq = (node->height * 0.5 * (node->height + 1)) * 10;
	if (node->left != NULL) {
		// Draw a line from the current node to the left node factoring in the height of the tree
		int xpos = x - xeq;
		int ypos = y + 100;
		GUI::line(x, y, xpos, ypos, new int[3]{0, 255, 0});
		drawTreeNode(node->left, xpos, ypos);
	}
	if (node->right != NULL) {
		// Draw a line from the current node to the right node factoring in the height of the tree
		int xpos = x + xeq;
		int ypos = y + 100;
		GUI::line(x, y, xpos, ypos, new int[3]{0, 255, 0});
		drawTreeNode(node->right, xpos, ypos);
	}

	// Add the node to the nodePositions array
	if (reIndexNodes) {
		nodePositionsSize++;
		nodePositions = (nodePos**)realloc(nodePositions, nodePositionsSize * sizeof(nodePos*));
		nodePositions[nodePositionsSize - 1] = new nodePos{x, y, node};
	}
}
