#include "test.h"
#include <array>
#include <map>
#include <list>
#include <vector>
#include <chrono>
using namespace std;
static bool benchmark = false;

bool testAVLTreeInsertion(InsertionTestEntry* testEntries, int length, chrono::duration<double, std::micro>* duration) {
	int failedTests = 0;
	std::cout << "Testing AVL tree insertion..." << endl;
	for (int i = 0; i < length; i++) {
		std::cout << "Test " << i + 1 << " [" << testEntries[i].name << "]: ";

		// Start a timer to measure the time it takes to insert the keys
		auto start = std::chrono::high_resolution_clock::now();

		// Initialize the tree and the expected tree
		AVLTree tree = AVLTree(testEntries[i].keys, testEntries[i].length);
		AVLNode* treeRoot = tree.root;

		// Stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		*duration += stop - start;

		// Check if the tree is balanced
		if (isAVLTreeBalanced(&tree, tree.root)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		// If the tree is not balanced, it is a failed test
		printf(RED "Failed\n" RESET);

		failedTests++;
	}

	std::cout << "AVL tree insertion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTreeDeletion(DeletionTestEntry* testEntries, int length, chrono::duration<double, std::micro>* duration) {
	int failedTests = 0;
	std::cout << "Testing AVL tree deletion..." << endl;
	for (int i = 0; i < length; i++) {
		std::cout << "Test " << i + 1 << " [" << testEntries[i].name << "]: ";

		// Initialize the tree
		AVLTree tree = AVLTree(testEntries[i].keys, testEntries[i].length);

		// Start a timer to measure the time it takes to delete the keys
		auto start = std::chrono::high_resolution_clock::now();

		// Delete the keys
		for (int j = 0; j < testEntries[i].deleteLength; j++) {
			int key = testEntries[i].deleteKeys[j];
			tree.deleteKey(key);
		}

		// Stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		*duration += stop - start;

		// Otherwise, Check if the correct nodes we're deleted

		// add all node to a list with addNodesToList

		int keysLength = tree.size;
		int* nodeKeysIntArray = new int[tree.size];
		getKeys(tree.root, nodeKeysIntArray, new int(0));

		// Convert the array to a list
		list<int> nodeKeys;
		for (int j = 0; j < keysLength; j++)
			nodeKeys.push_back(nodeKeysIntArray[j]);
		nodeKeys.sort();

		// Expected node keys when the deletions are applied
		list<int> expectedNodeKeys;
		list<int> deletedKeys;
		for (int j = 0; j < testEntries[i].deleteLength; j++)
			deletedKeys.push_back(testEntries[i].deleteKeys[j]);
		deletedKeys.sort();
		for (int j = 0; j < testEntries[i].length; j++)
			expectedNodeKeys.push_back(testEntries[i].keys[j]);
		expectedNodeKeys.sort();
		for (auto it = deletedKeys.begin(); it != deletedKeys.end(); it++)
			expectedNodeKeys.remove(*it);

		list<int> wrongDeletedKeys;
		list<int> notDeletedKeys;

		// Find all wrong deleted keys and all not deleted keys without using find
		for (auto it = nodeKeys.begin(); it != nodeKeys.end(); it++) {
			bool found = false;
			for (auto it2 = expectedNodeKeys.begin(); it2 != expectedNodeKeys.end(); it2++) {
				if (*it == *it2) {
					found = true;
					break;
				}
			}
			if (!found)
				wrongDeletedKeys.push_back(*it);
		}

		// Check if the difference is empty
		if (wrongDeletedKeys.size() != 0 && notDeletedKeys.size() != 0) {
			// If not, print the error message and information about the difference
			printf(RED "Key deletion failed\n" RESET);
			printf(RED "Failed to delete: " RESET);
			for (auto it = notDeletedKeys.begin(); it != notDeletedKeys.end(); it++)
				std::cout << *it << " ";
			std::cout << endl;
			printf(RED "Wrongly deleted: " RESET);
			for (auto it = wrongDeletedKeys.begin(); it != wrongDeletedKeys.end(); it++)
				std::cout << *it << " ";
			std::cout << endl;

			// printf("Deleted keys:\n");
			// for (auto it = nodeKeys.begin(); it != nodeKeys.end(); it++)
			// 	std::cout << it->first << " ";
			// std::cout << endl;

			// printf("Expected keys:\n");
			// for (auto it = expectedNodeKeys.begin(); it != expectedNodeKeys.end(); it++)
			// 	std::cout << it->first << " ";
			// std::cout << endl;

			// printf("Difference:\n");
			// for (auto it = difference.begin(); it != difference.end(); it++)
			// 	std::cout << it->first << " ";
			// std::cout << endl;

			failedTests++;
			continue;
		}

		// Check if the tree is not balanced
		if (!isAVLTreeBalanced(&tree, tree.root)) {
			printf(RED "Failed\n" RESET);
			printf(RED "The correct nodes were deleted, but the tree is still not balanced\n" RESET);
			failedTests++;
			continue;
		}

		// Else, the test passed
		printf(GREEN "Passed\n" RESET);
	}

	std::cout << "AVL tree deletion test finished with " << failedTests << " failed tests" << endl;

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
	int testLength = 10;
	if (useRandom) {
		testsAmount = 100;
		testLength = 1000;
		deleteTestsLength = insertTestsLength = testsAmount;
		insertTests = new InsertionTestEntry[testsAmount];
		deleteTests = new DeletionTestEntry[testsAmount];
		std::cout << "Generating " << testsAmount << " random tests..." << endl;
		for (int i = 0; i < testsAmount; i++) {
			// Generate a random test
			InsertionTestEntry* test = new InsertionTestEntry;
			test->name = "Random test " + to_string(i);
			test->keys = generateRandomArray(testLength, 0, 1000000);
			test->length = testLength;

			// Add the test to the array
			insertTests[i] = *test;

			// Generate a random deletion test
			DeletionTestEntry* deleteTest = new DeletionTestEntry;
			deleteTest->name = "Random test " + to_string(i);
			deleteTest->keys = test->keys;
			deleteTest->length = test->length;

			deleteTest->deleteLength = (int)(testLength * (rand() % 100) / 120.0) + 1;
			deleteTest->deleteKeys = generateRandomArray(deleteTest->deleteLength, 0, 1000);

			// Add the test to the array
			deleteTests[i] = *deleteTest;
		}
	}

	std::cout << "Testing AVL tree..." << endl;

	// Declare a duration variable
	chrono::duration<double, std::micro> duration = chrono::duration<double, std::micro>::zero();

	if (!testAVLTreeInsertion(insertTests, insertTestsLength, &duration)) {
		std::cout << "AVL Tree insertion failed!" << endl;
		std::cout << "Will not test deletion" << endl;
		return false;
	}
	// Stop timer for insertion
	std::cout << "AVL Tree insertion took " << duration.count() << " microseconds" << endl;
	std::cout << "Inserted " << testLength << " keys" << endl;
	std::cout << "That's " << (double)duration.count() / testLength << " microseconds per key" << endl;
	std::cout << "or " << (double)testLength / duration.count() << " keys per microsecond" << endl;

	// Info about how many tests were passed
	std::cout << "AVL Tree insertion passed!" << endl;

	// Restart timer for deletion
	duration = chrono::duration<double, std::micro>::zero();
	if (!testAVLTreeDeletion(deleteTests, deleteTestsLength, &duration)) {
		std::cout << "AVL Tree deletion failed!" << endl;
		return false;
	}
	std::cout << "AVL Tree deletion passed!" << endl;
	std::cout << "AVL Tree deletion took " << duration.count() << " microseconds" << endl;
	std::cout << "Deleted " << testLength << " keys" << endl;
	std::cout << "That's " << (double)duration.count() / testLength << " microseconds per key" << endl;
	std::cout << "or " << (double)testLength / duration.count() << " keys per microsecond" << endl;

	return true;
}

// Unnecessary function, but keeping it for now
AVLNode* codeToTree(string code) {
	// The first character is the root node
	AVLNode* root = createNode(code[0] - '0');

	// The rest of the characters are the children
	//  '/' means right node, '\' means left node
	AVLNode* current = root;
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
bool compareTrees(AVLNode* tree1, AVLNode* tree2) {
	if (tree1 == NULL && tree2 == NULL) return true;
	if (tree1 == NULL || tree2 == NULL) return false;
	if (tree1->key != tree2->key) return false;
	return compareTrees(tree1->left, tree2->left) && compareTrees(tree1->right, tree2->right);
}

// Go through each node and check if the balance is correct
static bool isAVLTreeBalanced(AVLTree* tree, AVLNode* node) {
	if (node == NULL) return true;
	if (abs(tree->getBalance(node)) > 1) return false;
	return (node->left == NULL || isAVLTreeBalanced(tree, node->left)) &&
		   (node->right == NULL || isAVLTreeBalanced(tree, node->right));
}

// get all keys in the tree
static void getKeys(AVLNode* node, int* keys, int* index) {
	if (node == NULL) return;
	getKeys(node->left, keys, index);
	keys[*index] = node->key;
	(*index)++;
	getKeys(node->right, keys, index);
}