#ifndef TEST_H
#define TEST_H

#include "../utils.h"
#include "../modules/AVLTree.h"
#include <vector>
#include <list>
using namespace std;

AVLNode* codeToTree(string code);
bool compareTrees(AVLNode* tree1, AVLNode* tree2);

static InsertionTestEntry* AVLInsertionTests = new InsertionTestEntry[10]{
	{new int[3]{1, 2, 3}, 3, "Simple left rotate test"},
	{new int[3]{3, 2, 1}, 3, "Simple right rotate test"},
	{new int[3]{1, 3, 2}, 3, "Left-right rotate test"},
	{new int[3]{3, 1, 2}, 3, "Right-left rotate test"},
	// More complex tests
	{new int[5]{1, 2, 3, 4, 5}, 5, "Complex test 1"},
	{new int[5]{5, 4, 3, 2, 1}, 5, "Complex test 2"},
	{new int[7]{5, 8, 7, 4, 7, 1, 3}, 7, "Complex test 3"},
	{new int[7]{1, 9, 4, 7, 3, 2, 6}, 7, "Complex test 4"},
	{new int[8]{1, 5, 7, 3, 2, 4, 6, 8}, 8, "Complex test 5"},
	{new int[31]{7, 1, 2, 9, 5, 3, 5, 1, 2, 6, 3, 3, 1, 1, 7, 2, 4, 5, 6, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 31, "Insane test"},
};

static DeletionTestEntry* AVLDeletionTests = new DeletionTestEntry[13]{
	{new int[3]{1, 2, 3}, 3, new int[1]{2}, 1, "Root node deletion test"},
	{new int[3]{3, 2, 1}, 3, new int[1]{3}, 1, "Right child node with no children deletion test"},
	{new int[4]{3, 2, 1, 4}, 4, new int[1]{3}, 1, "Right child node with a right child deletion test"},
	{new int[4]{1, 2, 4, 3}, 4, new int[1]{4}, 1, "Right child node with a left child deletion test"},
	{new int[5]{1, 2, 3, 4, 5}, 5, new int[1]{4}, 1, "Right child node with both children deletion test"},
	{new int[3]{1, 2, 3}, 3, new int[1]{1}, 1, "Left child node with no children deletion test"},
	{new int[4]{5, 2, 6, 3}, 4, new int[1]{2}, 1, "Left child node with a right child deletion test"},
	{new int[4]{4, 2, 3, 1}, 4, new int[1]{2}, 1, "Left child node with a left child deletion test"},
	{new int[5]{5, 2, 6, 3, 1}, 5, new int[1]{2}, 1, "Left child node with both children deletion test"},

	// More complex tests
	{new int[7]{5, 8, 7, 4, 7, 1, 3}, 7, new int[2]{4, 7}, 2, "Complex test 1"},
	{new int[7]{1, 9, 4, 7, 3, 2, 6}, 7, new int[3]{4, 3, 2}, 3, "Complex test 2"},
	{new int[8]{1, 5, 7, 3, 2, 4, 6, 8}, 8, new int[4]{3, 2, 1, 5}, 4, "Complex test 3"},
	{new int[31]{7, 1, 2, 9, 5, 3, 5, 1, 2, 6, 3, 3, 1, 1, 7, 2, 4, 5, 6, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 31, new int[11]{5, 5, 9, 1, 2, 3, 4, 6, 8, 1, 3}, 11, "Insane test"},
};

/**
 * @brief Tests the AVL tree insertion functionality
 *
 * @return true
 * @return false
 */
bool testAVLTreeInsertion(InsertionTestEntry* testEntries, int length);

/**
 * @brief Tests the AVL tree deletion functionality
 *
 * @return true if all tests passed
 * @return false if any test failed
 */
bool testAVLTreeDeletion(DeletionTestEntry* testEntries, int length);

/**
 * @brief Runs the AVL tree tests
 *
 * @return true if all tests passed
 * @return false if any test failed
 */
bool testAVLTree(bool);

/**
 * @brief Converts a string representation of a tree to a tree ( Unneeded but could be useful )
 *
 * @param code String representation of the tree
 * @return AVLNode* The root of the tree
 */
AVLNode* codeToTree(string);

// Compares two trees
bool compareTrees(AVLNode*, AVLNode*);

// Go through each node and check if the balance is correct
static bool isAVLTreeBalanced(AVLTree*, AVLNode*);

// Add each node to an array and return it
static void getKeys(AVLNode* node, int* keys, int* index);
#endif