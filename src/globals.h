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

// Define sizes of data types
#define SIZE_INT sizeof(int)
#define SIZE_STRING sizeof(std::string)
#define SIZE_STRING_POINTER sizeof(std::string *)
#define SIZE_VECTOR sizeof(std::vector<int>)

// Rotate directions used in AVLTree and RedBlackTree rotate functions
enum RotateDirection {
	LEFT,
	RIGHT
};

enum RandomizationType {
	UNIQUE_RANDOM,
	NON_UNIQUE_RANDOM,
};

enum TestType {
	BASIC,
	STRICT
};

enum BenchmarkType {
	ALL,
	FINAL,
};

enum DataStructureType {
	AVL,
	RedBlack,
	HashTableChaining,
	HashTableOpenAddressing
};

// AVLNode structure
struct AVLNode {
	int key;
	int value;
	AVLNode *parent;
	AVLNode *left;
	AVLNode *right;
	int count;
	int height;
	AVLNode();
	AVLNode(int key);
	AVLNode(int key, int value);
};

// RedBlackNode colors
enum RedBlackNodeColor {
	black,
	red
};

// RedBlackNode structure
struct RedBlackNode {
	int key;
	int value;
	RedBlackNode *parent;
	RedBlackNode *left;
	RedBlackNode *right;
	int count;
	RedBlackNodeColor color;
	RedBlackNode();
	RedBlackNode(int key, int value);
	RedBlackNode(int key, int value, RedBlackNodeColor color);
};

// Standard wrapper class for data structures
class DSStandardWrapper {
public:
	std::string name = "Example";
	DSStandardWrapper();
	~DSStandardWrapper();
	virtual void init(int);
	virtual void insert(int);
	virtual int search(int);
	virtual void returnlessSearch(int);
	virtual void remove(int);
	virtual bool isValid();
	virtual int getSize();
	virtual long long calculateSpaceComplexity();
	virtual void destroy();
};

#endif	// __GLOBALS_H__
