#ifndef GLOBALS_H
#define GLOBALS_H

// Colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#include <iostream>
#include <sstream>

// Tree node structure
struct Node {
	int key;
	Node *parent;
	Node *left;
	Node *right;
	int height;
};

Node *createNode(int key) {
	Node *newNode = new Node;
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	newNode->height = 1;
	return newNode;
}

// Used for test entries
struct InsertionTestEntry {
	int *keys;
	int length;
	std::string code;
	std::string name;
};

// Also used for test entries
struct DeletionTestEntry {
	int *keys;
	int length;
	int *deleteKeys;
	int deleteLength;
	std::string code;
	std::string name;
};

#endif