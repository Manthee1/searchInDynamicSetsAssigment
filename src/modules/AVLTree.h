#ifndef AVLTREE_H
#define AVLTREE_H

#include "../utils.h"

class AVLTree {
private:
	/**
	 *@brief Rotate the tree to the left
	 *
	 * @param x - Subtree root
	 * @return Node* - New root of the subtree
	 */
	Node *leftRotate(Node *);

	/**
	 *@brief Rotate the tree to the right
	 *
	 * @param y - Subtree root
	 * @return Node* - New root of the subtree
	 */
	Node *rightRotate(Node *);
	/**
	 *@brief Balance the tree starting from the given node (used after insertion)
	 *
	 * @param node - Starting node
	 * @param newKey - Key to be compared with
	 */
	void balanceTree(Node *, int);

	/**
	 *@brief Balance the tree starting from the given node (used after deletion)
	 *
	 * @param node
	 */
	void balanceTree(Node *);

public:
	Node *root;
	AVLTree();
	// Variadic constructor with std::initializer_list
	AVLTree(std::initializer_list<int>);
	// Constructor with array
	AVLTree(int *, int);
	~AVLTree();

	int getHeight(Node *);
	int getBalance(Node *);

	void insertKey(int);
	/**
	 * @brief Insert a node into the tree
	 *
	 * @param node - Node to be inserted
	 */
	void insertNode(Node *);

	/**
	 *@brief Search for a node with the given key
	 *
	 * @param key - Key to be searched for
	 * @return Node* - Pointer to the node with the given key
	 */
	Node *searchKey(int);

	void deleteKey(int);

	/**
	 *@brief Delete a node from the tree
	 *
	 * @param node - Node to be deleted
	 */
	void deleteNode(Node *);
};

#endif	// __AVLTREE_H__