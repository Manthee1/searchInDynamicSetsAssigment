#include "AVLTreeView.h"
#include <math.h>
#include <sstream>
#include <gui.h>
#include <cstdlib>
#define IG GUI::imgui

static AVLTree tree;
static AVLNodePos** nodePositions = NULL;
static int insertKey = 0;
static int removeKey = 0;
static int nodePositionsSize = 0;
static bool reIndexNodes = true;

static AVLNode* selectedNode = NULL;

static void resetNodeGUIData() {
	// Reset the node positions
	for (int i = 0; i < nodePositionsSize; i++) {
		delete nodePositions[i];
	}
	delete nodePositions;
	nodePositionsSize = 0;
	// Allocate memory for the node positions
	nodePositions = new AVLNodePos*[(tree.root == NULL) ? 0 : tree.size];
	reIndexNodes = true;
}

static bool isAVLTreeBalanced(AVLTree* tree, AVLNode* node) {
	if (node == NULL) return true;

	if (abs(tree->getBalance(node)) > 1) {
		std::cout << "AVLNode " << node->key << " is not balanced" << std::endl;
		return false;
	}
	return (node->left == NULL || isAVLTreeBalanced(tree, node->left)) &&
		   (node->right == NULL || isAVLTreeBalanced(tree, node->right));
}

void AVLTreeView::draw() {
	// Add a node to the tree
	IG::Begin("Add AVLNode");
	IG::InputInt("Key", &insertKey);

	if (IG::Button("Add AVLNode")) {
		tree.insertKey(insertKey);
		resetNodeGUIData();
	}
	if (IG::Button("Add AVLNode(Not balanced)")) {
		tree.insertNode(new AVLNode(insertKey), false);
		resetNodeGUIData();
	}
	IG::End();

	// Remove a node from the tree
	IG::Begin("Remove AVLNode");
	IG::InputInt("Key", &removeKey);
	if (IG::Button("Remove AVLNode")) {
		tree.deleteKey(removeKey);
		selectedNode = NULL;
		resetNodeGUIData();
	}
	if (IG::Button("Clear Tree")) {
		delete tree.root;
		selectedNode = NULL;
		tree.root = NULL;
		resetNodeGUIData();
	}
	IG::End();

	// Verify balance
	IG::Begin("Verify Balance");
	if (IG::Button("Verify Balance")) {
		std::cout << "Tree is balanced: " << isAVLTreeBalanced(&tree, tree.root) << std::endl;
	}
	IG::End();

	IG::Begin("Random Tree");
	static int size = 1000;
	IG::InputInt("Amount", &size);
	if (IG::Button("Create")) {
		delete tree.root;
		tree.root = NULL;
		for (int i = 0; i < size; i++)
			tree.insertKey(rand() % size);
		resetNodeGUIData();
	}
	static int deleteSize = 700;
	IG::InputInt("DeleteAmount", &deleteSize);
	if (IG::Button("Delete")) {
		for (int i = 0; i < deleteSize; i++)
			tree.deleteKey(rand() % 10000);
		resetNodeGUIData();
	}
	IG::End();

	IG::Begin("Selected AVLNode Info");
	if (selectedNode != NULL) {
		IG::Text("Key: %d", selectedNode->key);
		IG::Text("Height: %d", selectedNode->height);
		IG::Text("Balance: %d", tree.getBalance(selectedNode));
		IG::Text("Count: %d", selectedNode->count);
		// Right rotation
		if (IG::Button("Right Rotate")) {
			tree.rotate(selectedNode, RIGHT);
			resetNodeGUIData();
		}
		// Left rotation
		if (IG::Button("Left Rotate")) {
			tree.rotate(selectedNode, LEFT);
			resetNodeGUIData();
		}

		// Delete the selected node
		if (IG::Button("Delete AVLNode")) {
			tree.deleteNode(selectedNode);
			selectedNode = NULL;
			resetNodeGUIData();
		}

		// Run balance algorithm from selected node
		if (IG::Button("Init Balance")) {
			tree.balanceTree(selectedNode);
			resetNodeGUIData();
		}
	}

	IG::End();

	// Check if a node was clicked
	if (IG::IsMouseClicked(0)) {
		int x = GUI::getMouseX();
		int y = GUI::getMouseY();

		// Check if the mouse was clicked on a node
		for (int i = 0; i < nodePositionsSize; i++) {
			AVLNodePos* pos = nodePositions[i];
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

	// Draw the tree
	GUI::beginMain();
	drawTreeNode(tree.root, IG::GetIO().DisplaySize.x / 2, 50, tree.root == NULL ? 0 : tree.root->height);
	IG::End();
	reIndexNodes = false;
}

void AVLTreeView::drawTreeNode(AVLNode* node, int x, int y, int level) {
	int colorRed[3] = {255, 0, 0};
	int colorGreen[3] = {0, 255, 0};
	int colorBlue[3] = {0, 0, 255};
	int colorWhite[3] = {255, 255, 255};
	int colorBlack[3] = {0, 0, 0};

	if (node == NULL) return;
	// Draw the tree
	// Draw a circle at the current node
	int xeq = 4 * pow(2, level);
	if (GUI::isRectOnScreen(x - 20, y - 20, 40, 40)) {
		GUI::circle(x, y, 20, (selectedNode == node) ? colorGreen : colorRed);
		// Draw the key of the current node
		GUI::text(x - 10, y - 10, std::to_string(node->key), colorWhite);
	}
	if ((node->left != NULL && !GUI::isLeftOfScreen(x)) || reIndexNodes) {
		//  Draw a line from the current node to the left node factoring in the height of the tree
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

	// Add the node to the nodePositions array
	if (reIndexNodes) {
		nodePositions[nodePositionsSize] = new AVLNodePos{node, x, y};
		nodePositionsSize++;
	}
}
