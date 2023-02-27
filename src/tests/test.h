#ifndef TEST_H
#define TEST_H

#include "../utils.h"
#include "../modules/AVLTree.h"
#include "../modules/RedBlackTree.h"
#include "../modules/OAHashTable.h"
#include <vector>
#include <list>
using namespace std;

AVLNode* codeToTree(string code);
bool compareTrees(AVLNode* tree1, AVLNode* tree2);

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
bool testAVLTree();
void testRedBlackTree();
void testOAHashTable();

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

// Go through each node and check if the RedBlackTree is valid
static bool isRedBlackTreeValid(RedBlackTree*, RedBlackNode*);

// Add each node to an array and return it
static void getKeys(AVLNode* node, int* keys, int* index);
#endif