#ifndef AVLTree_CPP
#define AVLTree_CPP

#include "../globals.cpp"
#include "../utils.cpp"
#include <iostream>
using namespace std;

class AVLTree {
private:
	/**
	 *@brief Rotate the tree to the left
	 *
	 * @param x - Subtree root
	 * @return Node* - New root of the subtree
	 */
	Node *leftRotate(Node *x) {
		if (x->right == NULL) throw "No right child";
		Node *y = x->right;

		if (y->left != NULL) {
			Node *tempNode = y->left;
			x->right = tempNode;
			tempNode->parent = x;
		} else
			x->right = NULL;

		// If the parent of x is null, then x is the root. So set y as the root
		if (x->parent == NULL) root = y;
		// Otherwise, set y as the left or right child of x's parent
		else if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;

		y->parent = x->parent;
		x->parent = y;
		y->left = x;

		// Update heights
		x->height = 1 + max(getHeight(x->left), getHeight(x->right));
		y->height = 1 + max(getHeight(y->left), getHeight(y->right));

		// Return new root
		return y;
	}
	/**
	 *@brief Rotate the tree to the right
	 *
	 * @param y - Subtree root
	 * @return Node* - New root of the subtree
	 */

	Node *rightRotate(Node *y) {
		if (y->left == NULL) throw "No left child";
		Node *x = y->left;

		if (x->right != NULL) {
			Node *tempNode = x->right;
			y->left = tempNode;
			tempNode->parent = y;
		} else
			y->left = NULL;

		// If the parent of y is null, then y is the root. So set x as the root
		if (y->parent == NULL) root = x;
		// Otherwise, set x as the left or right child of y's parent
		else if (y->parent->right == y)
			y->parent->right = x;
		else
			y->parent->left = x;

		x->parent = y->parent;
		y->parent = x;
		x->right = y;

		// Update heights
		y->height = 1 + max(getHeight(y->left), getHeight(y->right));
		x->height = 1 + max(getHeight(x->left), getHeight(x->right));

		return x;
	}

	/**
	 *@brief Balance the tree starting from the given node (used after insertion)
	 *
	 * @param node - Starting node
	 * @param newKey - Key to be compared with
	 */
	void balanceTree(Node *node, int newKey) {
		if (node == NULL) return;

		int balance = getBalance(node);
		if (balance > 1) {
			if (newKey <= node->left->key)
				node = rightRotate(node);
			else {
				leftRotate(node->left);
				node = rightRotate(node);
			}
		} else if (balance < -1) {
			if (newKey >= node->right->key)
				node = leftRotate(node);
			else {
				rightRotate(node->right);
				node = leftRotate(node);
			}
		}

		balanceTree(node->parent, newKey);
	}

	/**
	 *@brief Balance the tree starting from the given node (used after deletion)
	 *
	 * @param node
	 */
	void balanceTree(Node *node) {
		if (node == NULL) return;

		int balance = getBalance(node);
		if (balance > 1) {
			if (getBalance(node->left) >= 0)
				node = rightRotate(node);
			else {
				leftRotate(node->left);
				node = rightRotate(node);
			}
		} else if (balance < -1) {
			if (getBalance(node->right) <= 0)
				node = leftRotate(node);
			else {
				rightRotate(node->right);
				node = leftRotate(node);
			}
		}

		balanceTree(node->parent);
	}

public:
	Node *root;
	AVLTree() {
		root = NULL;
	}
	// Variadic constructor with std::initializer_list
	AVLTree(std::initializer_list<int> list) {
		root = NULL;
		for (int key : list)
			insertKey(key);
	}
	// Constructor with array
	AVLTree(int *list, int size) {
		root = NULL;
		for (int i = 0; i < size; i++)
			insertKey(list[i]);
	}
	~AVLTree() { delete root; }

	int getHeight(Node *node) { return (node == NULL) ? 0 : node->height; }
	int getBalance(Node *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }

	void insertKey(int key) { insertNode(createNode(key)); }
	/**
	 * @brief Insert a node into the tree
	 *
	 * @param node - Node to be inserted
	 */
	void insertNode(Node *node) {
		if (node == NULL) return;

		// If the tree is empty, make the node the root
		if (root == NULL) {
			root = node;
			return;
		}
		// Find the appropriate parent for the node (make it a leaf)
		Node *current = root;
		Node *parent = NULL;
		while (current != NULL) {
			parent = current;
			if (node->key < current->key)
				current = current->left;
			else
				current = current->right;
		}

		// Insert the node depending on its key value
		if (node->key < parent->key)
			parent->left = node;
		else
			parent->right = node;

		// Set the parent
		node->parent = parent;

		// Update the height of the nodes on the path from the inserted node to the root
		while (parent != NULL) {
			parent->height = 1 + max(getHeight(parent->left), getHeight(parent->right));
			parent = parent->parent;
		}

		// Balance the tree
		balanceTree(node->parent, node->key);
	}

	/**
	 *@brief Search for a node with the given key
	 *
	 * @param key - Key to be searched for
	 * @return Node* - Pointer to the node with the given key
	 */
	Node *searchKey(int key) {
		Node *current = root;
		while (current != NULL) {
			if (current->key == key) return current;
			current = (key < current->key) ? current->left : current->right;
		}
		return NULL;
	}

	void deleteKey(int key) {
		Node *node = searchKey(key);
		deleteNode(node);
	}

	/**
	 *@brief Delete a node from the tree
	 *
	 * @param node - Node to be deleted
	 */
	void deleteNode(Node *node) {
		if (node == NULL) return;

		// If the node only has one direct child
		if ((node->right == NULL) ^ (node->left == NULL)) {
			// Set the child as the node's parent's left or right child
			Node *child = (node->right == NULL) ? node->left : node->right;
			// If the node is the root, set the child as the new root
			// Otherwise, set the child as the node's parent's left or right child
			if (node->parent == NULL)
				root = child;
			else if (node->parent->left == node) {
				node->parent->left = child;
				child->parent = node->parent;
			} else {
				node->parent->right = child;
				child->parent = node->parent;
			}
			delete node;
			return;
		}
		// If the node has both children
		// Here we don't delete the node, but we find the largest node in the left subtree and copy its key to the node to be "deleted"
		// After that, we delete the very node we copied the key from, because it has at most one child (easy to delete)
		if (node->right != NULL && node->left != NULL) {
			// Find the largest node in the left subtree
			Node *removeNode = node->left;
			while (removeNode->right != NULL)
				removeNode = removeNode->right;

			// Copy the largest subtree's node's key to the node to be deleted
			node->key = removeNode->key;

			// Make a pointer to the node to be deleted's parent
			Node *current, *tempLeafNode;
			current = tempLeafNode = removeNode->parent;

			// Delete the largest node
			deleteNode(removeNode);

			// use the current pointer to update the heights of the nodes on the path from the deleted node to the root
			while (current != NULL) {
				current->height = 1 + max(getHeight(current->left), getHeight(current->right));
				current = current->parent;
			}

			// Use the tempLeafNode pointer to balance the tree
			balanceTree(tempLeafNode);

			return;
		}

		// === If the node is a leaf ===
		// if the node is the root just remove it
		if (node->parent == NULL) {
			root = NULL;
			delete node;
			return;
		}

		// Otherwise find which child of the parent is the node and set it to null
		if (node->parent->left == node)
			node->parent->left = NULL;
		else
			node->parent->right = NULL;

		delete node;
	}
};

#endif