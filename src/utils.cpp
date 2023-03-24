#include "utils.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

int max(int a, int b) { return (a > b) ? a : b; }

int min(int a, int b) { return (a < b) ? a : b; }

void printTree(AVLNode* root, int level) {
	if (root == NULL) return;

	printTree(root->right, level + 1);
	if (level != 0) {
		for (int i = 0; i < level - 1; i++) std::cout << "|\t";
		std::cout << "|-------" << root->value << std::endl;
	} else
		std::cout << root->value << std::endl;

	printTree(root->left, level + 1);
}

int* generateRandomArray(int length, int min, int max) {
	int* array = new int[length];
	for (int i = 0; i < length; i++)
		array[i] = rand() % (max - min + 1) + min;
	return array;
	// remember to deallocate memory after use
	delete[] array;
}

int* generateRandomUniqueArray(int length, int min) {
	int* array = new int[length];
	for (int i = 0; i < length; i++) array[i] = i + min;
	shuffleInts(array, length);
	return array;
	// remember to deallocate memory after use
	delete[] array;
}

std::vector<int> generateRandomUniqueVectorArray(int length, int min, int max) {
	std::vector<int> keys(length);
	std::iota(keys.begin(), keys.end(), min);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(keys.begin(), keys.end(), g);
	keys.resize(length);
	return keys;
}

std::vector<int> generateRandomVectorArray(int length, int min, int max) {
	std::vector<int> array(length);
	for (int i = 0; i < length; i++) {
		array[i] = rand() % (max - min + 1) + min;
	}
	return array;
}

// Generate a random string of length n
std::string generateRandomString(int length) {
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string newstr;
	int pos;
	while (newstr.size() != (size_t)length) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}

void shuffleInts(int* array, int length) {
	for (int i = 0; i < length; i++) {
		int j = rand() % length;
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}