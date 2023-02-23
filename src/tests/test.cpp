#include "test.h"
#include <array>
#include <map>
using namespace std;

bool testAVLTreeInsertion() {
	int failedTests = 0;
	cout << "Testing AVL tree insertion..." << endl;
	for (int i = 0; i < 10; i++) {
		cout << "Test " << i + 1 << " [" << AVLInsertionTests[i].name << "]: ";

		// Initialize the tree and the expected tree
		AVLTree tree = AVLTree(AVLInsertionTests[i].keys, AVLInsertionTests[i].length);
		Node* treeRoot = tree.root;
		Node* codeRoot = codeToTree(AVLInsertionTests[i].code);

		// Compare the trees, if they are equal, the test passed
		if (compareTrees(treeRoot, codeRoot)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		// Otherwise, check if it is at least a balanced tree
		if (isAVLTreeBalanced(&tree, tree.root)) {
			printf(YELLOW "Passed (Balanced)\n" RESET);
			continue;
		}

		// If the trees are not equal, print the error message
		printf(RED "Failed\n" RESET);
		printf("Expected:\n");
		printTree(codeRoot, 0);
		printf("Got:\n");
		printTree(treeRoot, 0);

		failedTests++;
	}

	cout << "AVL tree insertion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTreeDeletion() {
	int failedTests = 0;
	cout << "Testing AVL tree deletion..." << endl;
	for (int i = 0; i < 13; i++) {
		cout << "Test " << i + 1 << " [" << AVLDeletionTests[i].name << "]: ";

		// Initialize the tree and apply the deletions
		AVLTree tree = AVLTree(AVLDeletionTests[i].keys, AVLDeletionTests[i].length);
		for (int j = 0; j < AVLDeletionTests[i].deleteLength; j++)
			tree.deleteKey(AVLDeletionTests[i].deleteKeys[j]);

		// Initialize the expected tree
		Node* codeRoot = codeToTree(AVLDeletionTests[i].code);

		// Compare the trees, if they are equal, the test passed
		if (compareTrees(tree.root, codeRoot)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}
		// Otherwise, check if it is at least a balanced tree
		if (isAVLTreeBalanced(&tree, tree.root)) {
			printf(YELLOW "Passed (Balanced)\n" RESET);
			continue;
		}

		// Otherwise, Check if the correct nodes we're deleted

		// Allocate a nodes array
		Node** nodes = new Node*[AVLDeletionTests[i].deleteLength];
		nodes = indexAvlTreeNodes(tree.root, nodes, new int[1]{0});
		// nodeKeys hash map of occurrences from nodes array
		map<int, int> nodeKeys;
		for (int j = 0; j < AVLDeletionTests[i].deleteLength; j++) {
			if (nodeKeys.find(nodes[j]->key) == nodeKeys.end())
				nodeKeys[nodes[j]->key] = 1;
			else
				nodeKeys[nodes[j]->key]++;
		}

		// Remove nodes as they are not used anymore
		delete[] nodes;

		// Expected occurrences hash map
		map<int, int> expectedNodeKeys;
		for (int j = 0; j < AVLDeletionTests[i].length; j++) {
			if (expectedNodeKeys.find(AVLDeletionTests[i].keys[j]) == expectedNodeKeys.end())
				expectedNodeKeys[AVLDeletionTests[i].keys[j]] = 1;
			else
				expectedNodeKeys[AVLDeletionTests[i].keys[j]]++;
		}

		// Find the difference between the two hash maps and append it to a new hash map
		map<int, int> difference;
		for (auto it = nodeKeys.begin(); it != nodeKeys.end(); it++) {
			if (expectedNodeKeys.find(it->first) == expectedNodeKeys.end())
				difference[it->first] = it->second;
			else if (expectedNodeKeys[it->first] != it->second)
				difference[it->first] = it->second - expectedNodeKeys[it->first];
		}

		// Check if the difference is empty
		if (difference.size() != 0) {
			// If not, print the error message and information about the difference
			printf(RED "Key deletion failed\n" RESET);
			printf("Deleted keys:\n");
			for (auto it = nodeKeys.begin(); it != nodeKeys.end(); it++)
				cout << it->first << " ";
			cout << endl;

			printf("Expected keys:\n");
			for (auto it = expectedNodeKeys.begin(); it != expectedNodeKeys.end(); it++)
				cout << it->first << " ";
			cout << endl;

			printf("Difference:\n");
			for (auto it = difference.begin(); it != difference.end(); it++)
				cout << it->first << " ";
			cout << endl;

			failedTests++;
			continue;
		}

		// If the difference is empty, print the error message
		printf(RED "Failed\n" RESET);
		printf(RED "The correct nodes were deleted, but the tree is still not balanced\n" RESET);
		printf("Expected:\n");
		printTree(codeRoot, 0);
		printf("Got:\n");
		printTree(tree.root, 0);

		failedTests++;
	}

	cout << "AVL tree deletion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTree() {
	if (!testAVLTreeInsertion()) {
		cout << "AVL Tree insertion failed!" << endl;
		cout << "Will not test deletion" << endl;
		return false;
	}
	cout << "AVL Tree insertion passed!" << endl;

	if (!testAVLTreeDeletion()) {
		cout << "AVL Tree deletion failed!" << endl;
		return false;
	}
	cout << "AVL Tree deletion passed!" << endl;

	return true;
}

Node* codeToTree(string code) {
	// The first character is the root node
	Node* root = createNode(code[0] - '0');

	// The rest of the characters are the children
	//  '/' means right node, '\' means left node
	Node* current = root;
	for (int i = 0; i < (int)code.length(); i++) {
		if (code[i] == '-') {
			current = current->parent;
			continue;
		}
		if (code[i] == '\\') {
			current->right = createNode(code[++i] - '0');
			current->right->parent = current;
			current = current->right;
		} else if (code[i] == '/') {
			current->left = createNode(code[++i] - '0');
			current->left->parent = current;
			current = current->left;
		}
	}
	return root;
}

// Compares two trees
bool compareTrees(Node* tree1, Node* tree2) {
	if (tree1 == NULL && tree2 == NULL) return true;
	if (tree1 == NULL || tree2 == NULL) return false;
	if (tree1->key != tree2->key) return false;
	return compareTrees(tree1->left, tree2->left) && compareTrees(tree1->right, tree2->right);
}

// Go through each node and check if the balance is correct
static bool isAVLTreeBalanced(AVLTree* tree, Node* node) {
	if (node == NULL) return true;
	if (abs(tree->getBalance(node)) > 1) return false;
	return (node->left == NULL || isAVLTreeBalanced(tree, node->left)) &&
		   (node->right == NULL || isAVLTreeBalanced(tree, node->right));
}

// Add each node to an array and return it
Node** indexAvlTreeNodes(Node* node, Node** nodes, int* index) {
	if (node == NULL) return nodes;
	nodes[*index] = node;
	(*index)++;
	nodes = indexAvlTreeNodes(node->left, nodes, index);
	nodes = indexAvlTreeNodes(node->right, nodes, index);
	return nodes;
}