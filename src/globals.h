#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

// Colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

// Tree node structure
struct Node {
	int key;
	Node *parent;
	Node *left;
	Node *right;
	int count;
	int height;
};

Node *createNode(int);

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

#endif	// __GLOBALS_H__