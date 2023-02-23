#include "AVLTreeView.h"
#include <math.h>
#include <sstream>
#include <gui.h>
#include <cstdlib>
#define IG GUI::imgui

static int insertKey = 0;
static int removeKey = 0;
static nodePos** nodePositions = NULL;
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
// 11 17
//  5 18
static bool isAVLTreeBalanced(AVLTree* tree, Node* node) {
	if (node == NULL) return true;

	if (abs(tree->getBalance(node)) > 1) {
		std::cout << "Node " << node->key << " is not balanced" << std::endl;
		return false;
	}
	return (node->left == NULL || isAVLTreeBalanced(tree, node->left)) &&
		   (node->right == NULL || isAVLTreeBalanced(tree, node->right));
}

static Node* selectedNode = NULL;
static AVLTree tree;
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
		selectedNode = NULL;
		resetNodeGUIData();
	}
	if (IG::Button("Clear Tree")) {
		delete tree.root;
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

	IG::Begin("Create Random Tree");
	static int size = 100;
	IG::InputInt("Amount", &size);
	if (IG::Button("Create Table")) {
		delete tree.root;
		tree.root = NULL;
		for (int i = 0; i < size; i++)
			tree.insertKey(rand() % 1000);
		resetNodeGUIData();
	}
	IG::End();

	IG::Begin("Selected Node Info");
	IG::Text("Key: %d", selectedNode == NULL ? 0 : selectedNode->key);
	IG::Text("Height: %d", selectedNode == NULL ? 0 : selectedNode->height);
	IG::Text("Balance: %d", selectedNode == NULL ? 0 : tree.getBalance(selectedNode));
	IG::Text("Count: %d", selectedNode == NULL ? 0 : selectedNode->count);
	// Right rotation
	if (IG::Button("Right Rotate")) {
		tree.rightRotate(selectedNode);
		resetNodeGUIData();
	}
	// Left rotation
	if (IG::Button("Left Rotate")) {
		tree.leftRotate(selectedNode);
		resetNodeGUIData();
	}

	// Delete the selected node
	if (IG::Button("Delete Node")) {
		tree.deleteNode(selectedNode);
		selectedNode = NULL;
		resetNodeGUIData();
	}

	// Run balance algorithm from selected node
	if (IG::Button("Init Balance")) {
		tree.balanceTree(selectedNode);
		resetNodeGUIData();
	}

	IG::End();

	// Check if a node was clicked
	if (IG::IsMouseClicked(0)) {
		int x = GUI::getMouseX();
		int y = GUI::getMouseY();

		// Check if the mouse was clicked on a node
		for (int i = 0; i < nodePositionsSize; i++) {
			nodePos* pos = nodePositions[i];
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

void AVLTreeView::drawTreeNode(Node* node, int x, int y, int level) {
	if (node == NULL) return;
	// Draw the tree
	// Draw a circle at the current node
	int xeq = 4 * pow(2, level);
	if (GUI::isRectOnScreen(x - 20, y - 20, 40, 40)) {
		if (selectedNode == node) {
			GUI::circle(x, y, 20, new int[3]{0, 255, 0});
		} else
			GUI::circle(x, y, 20, new int[3]{255, 0, 0});
		// Draw the key of the current node
		GUI::text(x - 10, y - 10, std::to_string(node->key), new int[3]{255, 255, 255});
	}
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

	// Add the node to the nodePositions array
	if (reIndexNodes) {
		nodePositionsSize++;
		nodePositions = (nodePos**)realloc(nodePositions, nodePositionsSize * sizeof(nodePos*));
		nodePositions[nodePositionsSize - 1] = new nodePos{x, y, node};
	}
}
