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

bool isPrime(int n) {
	if (n <= 1) return false;
	if (n <= 3) return true;
	if (n % 2 == 0 || n % 3 == 0) return false;
	for (int i = 5; i * i <= n; i = i + 6)
		if (n % i == 0 || n % (i + 2) == 0) return false;

	return true;
}

int* generateRandomArray(int length, int min, int max) {
	int* array = new int[length];
	for (int i = 0; i < length; i++)
		array[i] = rand() % (max - min + 1) + min;
	return array;
}
