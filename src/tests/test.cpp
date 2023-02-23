#include "test.h"
#include <array>
#include <map>
#include <chrono>
using namespace std;

bool testAVLTreeInsertion(InsertionTestEntry* testEntries, int length) {
	int failedTests = 0;
	cout << "Testing AVL tree insertion..." << endl;
	for (int i = 0; i < length; i++) {
		cout << "Test " << i + 1 << " [" << testEntries[i].name << "]: ";

		// Initialize the tree and the expected tree
		AVLTree tree = AVLTree(testEntries[i].keys, testEntries[i].length);
		Node* treeRoot = tree.root;

		// Check if the tree is balanced
		if (isAVLTreeBalanced(&tree, tree.root)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		// If the tree is not balanced, it is a failed test
		printf(RED "Failed\n" RESET);

		failedTests++;
	}

	cout << "AVL tree insertion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTreeDeletion(DeletionTestEntry* testEntries, int length) {
	int failedTests = 0;
	cout << "Testing AVL tree deletion..." << endl;
	for (int i = 0; i < length; i++) {
		cout << "Test " << i + 1 << " [" << testEntries[i].name << "]: ";

		// Initialize the tree and apply the deletions
		AVLTree tree = AVLTree(testEntries[i].keys, testEntries[i].length);
		for (int j = 0; j < testEntries[i].deleteLength; j++)
			tree.deleteKey(testEntries[i].deleteKeys[j]);

		// Check if the tree is balanced
		if (isAVLTreeBalanced(&tree, tree.root)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		// Otherwise, Check if the correct nodes we're deleted

		// Allocate a nodes array
		Node** nodes = new Node*[testEntries[i].deleteLength];
		nodes = indexAvlTreeNodes(tree.root, nodes, new int[1]{0});
		// nodeKeys hash map of occurrences from nodes array
		map<int, int> nodeKeys;
		for (int j = 0; j < testEntries[i].deleteLength; j++) {
			if (nodeKeys.find(nodes[j]->key) == nodeKeys.end())
				nodeKeys[nodes[j]->key] = 1;
			else
				nodeKeys[nodes[j]->key]++;
		}

		// Remove nodes as they are not used anymore
		delete[] nodes;

		// Expected occurrences hash map
		map<int, int> expectedNodeKeys;
		for (int j = 0; j < testEntries[i].length; j++) {
			if (expectedNodeKeys.find(testEntries[i].keys[j]) == expectedNodeKeys.end())
				expectedNodeKeys[testEntries[i].keys[j]] = 1;
			else
				expectedNodeKeys[testEntries[i].keys[j]]++;
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

		failedTests++;
	}

	cout << "AVL tree deletion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTree(bool useRandom) {
	// If useRandom is true, generate random tests

	// Insertion tests
	InsertionTestEntry* insertTests = AVLInsertionTests;
	int insertTestsLength = 10;
	DeletionTestEntry* deleteTests = AVLDeletionTests;
	int deleteTestsLength = 13;

	int testsAmount = 10;
	int testLength = 1000;
	if (useRandom) {
		testsAmount = 100;
		testLength = 100000;
		deleteTestsLength = insertTestsLength = testsAmount;
		insertTests = new InsertionTestEntry[testsAmount];
		deleteTests = new DeletionTestEntry[testsAmount];
		for (int i = 0; i < testsAmount; i++) {
			// Generate a random test
			InsertionTestEntry* test = new InsertionTestEntry;
			test->name = "Random test " + to_string(i);
			test->keys = generateRandomArray(testLength);
			test->length = testLength;

			// Add the test to the array
			insertTests[i] = *test;

			// Generate a random deletion test
			DeletionTestEntry* deleteTest = new DeletionTestEntry;
			deleteTest->name = "Random test " + to_string(i);
			deleteTest->keys = test->keys;
			deleteTest->length = test->length;
			deleteTest->deleteKeys = generateRandomArray((int)(testLength * (rand() % 100) / 120.0 || 1));
			deleteTest->deleteLength = testLength;

			// Add the test to the array
			deleteTests[i] = *deleteTest;
		}
	}

	cout << "Testing AVL tree..." << endl;
	// Start timer for insertion
	auto start = chrono::high_resolution_clock::now();
	if (!testAVLTreeInsertion(insertTests, insertTestsLength)) {
		cout << "AVL Tree insertion failed!" << endl;
		cout << "Will not test deletion" << endl;
		return false;
	}
	// Stop timer for insertion
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << "AVL Tree insertion took " << duration.count() << " microseconds" << endl;
	cout << "Inserted " << testLength << " keys" << endl;

	// Info about how many tests were passed
	cout << "AVL Tree insertion passed!" << endl;

	if (!testAVLTreeDeletion(deleteTests, deleteTestsLength)) {
		cout << "AVL Tree deletion failed!" << endl;
		return false;
	}
	cout << "AVL Tree deletion passed!" << endl;

	return true;
}

// Unnecessary function, but keeping it for now
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