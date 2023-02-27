#ifndef AVLTREE_H
#define AVLTREE_H

#include "../utils.h"

enum RotateDirection {
	LEFT,
	RIGHT
};

class AVLTree {
private:
	/**
	 *@brief Delete the tree starting from the given node
	 *
	 * @param node - Starting node
	 */
	void deleteTree(AVLNode *);

public:
	int getHeight(AVLNode *);
	int getBalance(AVLNode *);

	// The amount of nodes in the tree
	int size;

	/**
	 *@brief Rotate the subtree starting from the given node
	 *
	 * @param x - Subtree root
	 * @return AVLNode* - New root of the subtree
	 */
	AVLNode *rotate(AVLNode *, RotateDirection);

	/**
	 *@brief Balance the tree starting from the given node (used after insertion)
	 *
	 * @param node - Starting node
	 * @param newKey - Key to be compared with
	 */
	void balanceTree(AVLNode *, int);

	/**
	 *@brief Balance the tree starting from the given node (used after deletion)
	 *
	 * @param node
	 */
	void balanceTree(AVLNode *);

	AVLNode *root;
	AVLTree();
	// Variadic constructor with std::initializer_list
	AVLTree(std::initializer_list<int>);
	// Constructor with array
	AVLTree(int *, int);
	~AVLTree();

	void insertKey(int);
	/**
	 * @brief Insert a node into the tree
	 *
	 * @param node - AVLNode to be inserted
	 */
	void insertNode(AVLNode *, bool);

	/**
	 *@brief Search for a node with the given key
	 *
	 * @param key - Key to be searched for
	 * @return AVLNode* - Pointer to the node with the given key
	 */
	AVLNode *searchKey(int);

	void deleteKey(int);

	/**
	 *@brief Delete a node from the tree
	 *
	 * @param node - AVLNode to be deleted
	 */
	void deleteNode(AVLNode *);
};

#endif	// __AVLTREE_H__