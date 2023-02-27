#ifndef UTILS_H
#define UTILS_H
#include "globals.h"

int max(int, int);

int min(int, int);

void printTree(AVLNode*, int);

bool isPrime(int);

int* generateRandomArray(int, int, int);
#endif	// __UTILS_H__