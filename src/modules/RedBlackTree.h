#ifndef RedBlackTree_h
#define RedBlackTree_h

#include "../utils.h"

class RedBlackTree {
private:
	/**
	 * @brief Delete the tree starting from the given node
	 *
	 * @param node - Starting node
	 */
	void deleteTree(RedBlackNode *);

public:
	RedBlackNode *root;
	RedBlackTree();
	// Variadic constructor with std::initializer_list
	RedBlackTree(std::initializer_list<int>, std::initializer_list<int>);
	// Constructor with array
	RedBlackTree(int *, int *, int);
	~RedBlackTree();

	// The amount of nodes in the tree
	int size;
	// The height of the tree (Used for drawing)
	int height;

	/**
	 * @brief Rotate the subtree starting from the given node
	 *
	 * @param x - Subtree root
	 * @return RedBlackNode* - New root of the subtree
	 */
	RedBlackNode *rotate(RedBlackNode *, RotateDirection);

	/**
	 * @brief Transplant the subtree starting from the given node
	 *
	 * @param node - Subtree root
	 */
	void transplant(RedBlackNode *, RedBlackNode *);

	/**
	 * @brief Get the successor of the given node
	 *
	 * @param node - Node to get the successor of
	 * @return RedBlackNode* - Successor of the given node
	 */
	RedBlackNode *successor(RedBlackNode *);

	/**
	 * @brief Balance the tree starting from the given node (used after insertion)
	 *
	 * @param node - Starting node
	 * @param newKey - Key to be compared with
	 */
	void fixInsertFrom(RedBlackNode *);

	/**
	 * @brief Balance the tree starting from the given node (used after insertion)
	 *
	 * @param node - Starting node
	 * @param newKey - Key to be compared with
	 */
	void balanceFrom(RedBlackNode *, int);

	/**
	 * @brief Balance the tree starting from the given node (used after deletion)
	 *
	 * @param node
	 */
	void fixDeleteFrom(RedBlackNode *);

	/**
	 * @brief Insert a key into the tree
	 *
	 * @param key - Key to be inserted
	 */
	void insert(int, int);

	/**
	 * @brief Insert a node into the tree
	 *
	 * @param node - RedBlackNode to be inserted
	 */
	void insertNode(RedBlackNode *);

	/**
	 * @brief Search for a node with the given key
	 *
	 * @param key - Key to be searched for
	 * @return RedBlackNode* - Pointer to the node with the given key
	 */
	RedBlackNode *searchKey(int);

	/**
	 *@brief Search for a node with the given key
	 *
	 * @param key - Key to be searched for
	 */
	void deleteKey(int);

	/**
	 * @brief Delete a node from the tree
	 *
	 * @param node - RedBlackNode to be deleted
	 */
	void deleteNode(RedBlackNode *);
};

#endif	// __AVLTREE_H__