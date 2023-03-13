# I. Introduction
In this project, we will be comparing the performance of 4 data structures: AVL Tree, Red-Black Tree, Chaining Hash Table, and Open Addressing Hash Table (double hashing). These algorithms are important for all kinds of aplication in computer sicience like databases, file systems, and many more. 
The goal of this project is to compare the performance of these data structures in terms of time and space complexity.

# II. Background

Balanced trees and hash tables are two important data structures in computer science that are commonly used for efficient data storage and retrieval. 

A balanced tree is a data structure that maintains a balanced shape, ensuring that the height of the tree is proportional to the logarithm of the number of elements stored in it. This makes it possible to perform insertion, deletion, and search operations in O(log n) time, where n is the number of elements in the tree.

A hash table, on the other hand, is a data structure that uses a hash function to map keys to indices in an array. This allows for fast insertion, deletion, and search operations in O(1) time, on average.

In this project we used AVL Tree, Red-Black Tree, Chaining Hash Table, and Open Addressing Hash Table (double hashing) as our data structures and implemented them in C++. These implementations aim to provide a balance between simplicity and performance, while adhering to standard programming practices and guidelines. In the following sections, we will describe our implementations in detail, including their technical details, algorithms, and design choices. We will also present our testing methodology and results, and discuss the strengths and weaknesses of each data structure.


# III. Technical Details
## A. AVL Tree
The avl tree is a self-balancing binary search tree. It is named after its two inventors, Georgy Adelson-Velsky and Evgenii Landis. The AVL tree was the first such data structure to be invented. In an AVL tree, the heights of the two child subtrees of any node differ by at most one; if at any time they differ by more than one, rebalancing is done to restore this property. Lookup, insertion, and deletion all take O(log n) time in both the average and worst cases, where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be rebalanced by one or more tree rotations.
Explaining the implementation of AVL tree is a bit complicated, so we will just give a link to a really good explanation of it: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/

## B. Red-Black Tree
Much like the AVL tree, the red-black tree is a self-balancing binary search tree. It is named after the color used to represent its two colors in its node representation. In a red-black tree, each node is colored either red or black. By constraining how unbalanced the tree can become in certain cases, the red-black tree ensures that the height of the tree remains O(log n) in the average case, where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be rebalanced by one or more tree rotations.
The red-black tree must conform to the following red-black properties inorder to remain balanced:
1. Every node is either red or black.
2. The root is black.
3. Every leaf (NIL) is black (or in the case of this implementation, NULL)
4. If a node is red, then both its children are black.
5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.

## C. Chaining Hash Table
A hash table is a data structure that uses a hash function to map keys to indices in an array. This allows for fast insertion, deletion, and search operations in O(1) time, on average. In this implementation, we use chaining to resolve collisions. Chaining is a method of resolving collisions in which each index of the hash table's array is a pointer to a linked list. When a collision occurs, the new key-value pair is inserted into the linked list at the corresponding index. This allows for O(1) insertion, deletion, and search operations in the average case, but O(n) in the worst case, where n is the number of elements in the linked list at the corresponding index.

## D. Open Addressing Hash Table (double hashing)
Open addressing is a method of resolving collisions in which each index of the hash table's array is either empty or contains a key-value pair. When a collision occurs, the hash table probes the array for an empty slot using a probing function. If an empty slot is found, the new key-value pair is inserted into the array at that index. If no empty slot is found, the probing function is applied again to find a new index to probe. This process is repeated until an empty slot is found or the probing function returns an index that has already been probed. This allows for O(1) insertion, deletion, and search operations in the average case, but O(n) in the worst case, where n is the number of elements in the hash table.

# IV. Implementation
## A. AVL Tree
## B. Red-Black Tree
## C. Chaining Hash Table
## D. Open Addressing Hash Table (double hashing)
# V. Testing
# VI. Conclusion


