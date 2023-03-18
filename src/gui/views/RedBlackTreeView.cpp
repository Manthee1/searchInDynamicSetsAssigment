#include "RedBlackTreeView.h"
#include <math.h>
#include <sstream>
#include <vector>
#include <gui.h>
#include <cstdlib>
#define IG GUI::imgui

static RedBlackTree tree;
static std::vector<RBnodePos*> nodePositions;
static int nodePositionsSize = 0;
static bool reIndexNodes = true;
static int insertKey = 0;
static int insertValue = 0;
static int removeKey = 0;
static int treeHeight = 0;
static RedBlackNode* selectedNode = NULL;

// Get tree height
static int getTreeHeight(RedBlackNode* node) {
	if (node == NULL) return 0;
	return 1 + std::max(getTreeHeight(node->left), getTreeHeight(node->right));
}
static void updateTreeHeight(RedBlackNode* node) {
	treeHeight = getTreeHeight(node);
}

static void resetNodeGUIData() {
	updateTreeHeight(tree.root);
	for (int i = 0; i < nodePositionsSize; i++)
		delete nodePositions[i];

	nodePositionsSize = 0;
	nodePositions.clear();
	reIndexNodes = true;
}

void RedBlackTreeView::draw() {
	IG::Begin("RedBlackTree");

	IG::Text("Size: %d", tree.size);
	IG::Text("Tree Height: %d", treeHeight);
	IG::Separator();

	static int size = 1000;
	IG::InputInt("Amount", &size);
	if (IG::Button("Create")) {
		delete tree.root;
		tree.root = NULL;
		tree = RedBlackTree();
		int* keys = generateRandomUniqueArray(size, 0);
		for (int i = 0; i < size; i++) {
			tree.insert(keys[i], rand() % size);
		}
		resetNodeGUIData();
	}
	IG::Separator();

	IG::InputInt("Add Key", &insertKey);
	IG::InputInt("Add Value", &insertValue);

	if (IG::Button("Add RedBlackNode")) {
		tree.insert(insertKey, insertValue);
		resetNodeGUIData();
	}

	IG::Separator();

	// Seach
	static int searchKey = 0;
	IG::InputInt("Search Key", &searchKey);
	if (IG::Button("Search RedBlackNode"))
		selectedNode = tree.searchKey(searchKey);

	IG::Separator();

	IG::InputInt("Remove Key", &removeKey);
	if (IG::Button("Remove RedBlackNode")) {
		tree.deleteKey(removeKey);
		resetNodeGUIData();
	}
	if (IG::Button("Clear Tree")) {
		delete tree.root;
		tree.root = NULL;
		selectedNode = NULL;
		resetNodeGUIData();
	}

	IG::Separator();

	IG::End();

	if (selectedNode != NULL) {
		IG::Begin("Selected RedBlackNode");
		IG::Text("Key: %d", selectedNode->key);
		IG::Text("Value: %d", selectedNode->value);
		IG::Text("Count: %d", selectedNode->count);
		IG::Text("Color: %s", selectedNode->color == red ? "RED" : "BLACK");
		// Right rotation
		if (selectedNode->left != NULL && IG::Button("Right Rotate")) {
			tree.rotate(selectedNode, RIGHT);
			resetNodeGUIData();
		}
		// Left rotation
		if (selectedNode->right != NULL && IG::Button("Left Rotate")) {
			tree.rotate(selectedNode, LEFT);
			resetNodeGUIData();
		}

		// Delete the selected node
		if (IG::Button("Delete Node")) {
			tree.deleteNode(selectedNode);
			selectedNode = NULL;
			resetNodeGUIData();
		}

		// Run balance algorithm from selected node
		if (IG::Button("Init fix from node")) {
			tree.fixInsertFrom(selectedNode);
			resetNodeGUIData();
		}
		IG::End();
	}

	// Check if a node was clicked
	if (IG::IsMouseClicked(0)) {
		int x = GUI::getMouseX();
		int y = GUI::getMouseY();

		for (int i = 0; i < nodePositionsSize; i++) {
			RBnodePos* pos = nodePositions[i];
			// use sqrt to calculate the distance between the mouse and the node
			if (sqrt(pow(pos->x - x, 2) + pow(pos->y - y, 2)) < 20) {
				// The mouse was clicked on a node
				// std::cout << "Clicked on node with key " << pos->node->key << std::endl;
				// tree.deleteNode(pos->node);
				// resetNodeGUIData();

				selectedNode = pos->node;
				break;
			}
		}
	}

	GUI::beginMain();
	drawTreeNode(tree.root, GUI::getWidth() / 2, 50, treeHeight);
	IG::End();
	reIndexNodes = false;
}

void RedBlackTreeView::drawTreeNode(RedBlackNode* node, int x, int y, int level) {
	if (node == NULL) return;

	int colorRed[3] = {255, 0, 0};
	int colorSelectedRed[3] = {200, 50, 50};
	int colorBlack[3] = {0, 0, 0};
	int colorSelectedBlack[3] = {100, 100, 100};
	int colorWhite[3] = {255, 255, 255};

	// Draw the node
	// GUI::circle(x, y, 20, node->color == red ? colorRed : colorBlack);
	GUI::circle(x, y, 20, node->color == red ? node == selectedNode ? colorSelectedRed : colorRed : node == selectedNode ? colorSelectedBlack
																														 : colorBlack);
	GUI::text(x - 10, y - 10, std::to_string(node->key), colorWhite);

	int xeq = 4 * pow(2, level);

	if ((node->left != NULL && !GUI::isLeftOfScreen(x)) || reIndexNodes) {
		// Draw a line from the current node to the left node factoring in the height of the tree
		// If the line is not on the screen, don't draw it
		if (!GUI::isRightOfScreen(x - xeq) && !GUI::isLeftOfScreen(x))
			GUI::line(x, y, x - xeq, y + 100, colorWhite);
		// Draw the left node
		drawTreeNode(node->left, x - xeq, y + 100, level - 1);
	}
	if ((node->right != NULL && !GUI::isRightOfScreen(x)) || reIndexNodes) {
		// Draw a line from the current node to the right node factoring in the height of the tree
		// If the line is not on the screen, don't draw it
		if (!GUI::isLeftOfScreen(x + xeq) && !GUI::isRightOfScreen(x))
			GUI::line(x, y, x + xeq, y + 100, colorWhite);
		// Draw the right node
		drawTreeNode(node->right, x + xeq, y + 100, level - 1);
	}

	if (reIndexNodes) {
		nodePositions.push_back(new RBnodePos{node, x, y});
		nodePositionsSize++;
	}
}
