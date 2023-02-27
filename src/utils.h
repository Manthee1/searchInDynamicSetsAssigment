#ifndef UTILS_H
#define UTILS_H
#include "globals.h"
#include <vector>

int max(int, int);

int min(int, int);

void printTree(AVLNode*, int);

bool isPrime(int);

int* generateRandomArray(int, int, int);

std::vector<int> generateRandomUniqueArray(int length, int min, int max);

std::vector<int> generateRandomVectorArray(int, int, int);

#endif	// __UTILS_H__