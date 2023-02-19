#include "utils.h"
#include <iostream>

int max(int a, int b) { return (a > b) ? a : b; }

int min(int a, int b) { return (a < b) ? a : b; }

void printTree(Node* root, int level) {
	if (root == NULL) return;
	printTree(root->right, level + 1);
	for (int i = 0; i < level; i++) std::cout << "   ";
	std::cout << root->key << std::endl;
	printTree(root->left, level + 1);
}
