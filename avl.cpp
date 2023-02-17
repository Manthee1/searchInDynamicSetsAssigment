#include "utils.cpp"
#include <iostream>
using namespace std;

struct Node {
	int key;
	Node *parent;
	Node *left;
	Node *right;
	int height;
};

Node *createNode(int data) {
	Node *newNode = new Node;
	newNode->key = data;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	newNode->height = 1;
	return newNode;
}

class AVLTree {
public:
	Node *root;
	AVLTree() { root = NULL; }
	~AVLTree() { delete root; }

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

	int getHeight(Node *node) { return (node == NULL) ? 0 : node->height; }
	int getBalance(Node *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }

	void insert(int node) {
		insert(createNode(node));
	}
	void insert(Node *node) {
		if (node == NULL) return;

		// If the tree is empty, make the node the root
		if (root == NULL) {
			root = node;
			return;
		}
		// Find the leaf
		Node *current = root;
		Node *parent = NULL;
		while (current != NULL) {
			parent = current;
			if (node->key < current->key)
				current = current->left;
			else
				current = current->right;
		}

		// Insert the node
		if (node->key < parent->key)
			parent->left = node;
		else
			parent->right = node;

		// Set the parent
		node->parent = parent;

		// Balance the tree
		balanceTree(node->parent, node->key);
	}

	void balanceTree(Node *node, int newKey) {
		if (node == NULL) return;
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));

		int balance = getBalance(node);
		if (balance > 1) {
			if (newKey <= node->left->key)
				node = rightRotate(node);
			else {
				node = leftRotate(node->left);
				node = rightRotate(node);
			}
		} else if (balance < -1) {
			if (newKey >= node->right->key)
				node = leftRotate(node);
			else {
				node = rightRotate(node->right);
				node = leftRotate(node);
			}
		}

		balanceTree(node->parent, newKey);
	}

	Node *search(int key) {
		Node *current = root;
		while (current != NULL) {
			if (current->key == key) return current;
			current = (key < current->key) ? current->left : current->right;
		}
		return NULL;
	}

	void del(int key) {
		Node *node = search(key);
		del(node);
	}

	void del(Node *node) {
		if (node == NULL) return;

		if (node->right == NULL ^ node->left == NULL) {
			// If the node only has one direct child
			// Set the child as the node's parent's left or right child
			Node *child = (node->right == NULL) ? node->left : node->right;
			if (node->parent == NULL)
				root = child;
			else if (node->parent->left == node)
				node->parent->left = child;
			else
				node->parent->right = child;
			delete node;
			return;
		}
		// if the node has both children
		if (node->right != NULL && node->left != NULL) {
			// Find the largest node in the left subtree
			Node *current = node->left;
			while (current->right != NULL)
				current = current->right;

			// Copy the largest node's key to the node
			node->key = current->key;

			// Delete the largest node
			del(current);
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
