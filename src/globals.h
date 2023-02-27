#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

// Colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Rotate directions used in AVLTree and RedBlackTree rotate functions
enum RotateDirection {
	LEFT,
	RIGHT
};

// AVLNode structure
struct AVLNode {
	int key;
	AVLNode *parent;
	AVLNode *left;
	AVLNode *right;
	int count;
	int height;
	AVLNode();
	AVLNode(int key);
};

// RedBlackNode colors
enum RedBlackNodeColor {
	black,
	red
};

// RedBlackNode structure
struct RedBlackNode {
	int key;
	RedBlackNode *parent;
	RedBlackNode *left;
	RedBlackNode *right;
	int count;
	RedBlackNodeColor color;
	RedBlackNode();
	RedBlackNode(int key);
	RedBlackNode(int key, RedBlackNodeColor color);
};

// Used for test entries
struct InsertionTestEntry {
	int *keys;
	int length;
	std::string name;
};

// Also used for test entries
struct DeletionTestEntry {
	int *keys;
	int length;
	int *deleteKeys;
	int deleteLength;
	std::string name;
};

// Benchmark data used to specify which data structure's functions to use for benchmarking
struct BenchmarkData {
	std::string name;
	void (*init)();
	void (*insert)(int);
	void (*search)(int);
	void (*remove)(int);
	void (*destroy)();
};

#endif	// __GLOBALS_H__