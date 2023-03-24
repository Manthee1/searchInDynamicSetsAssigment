# I. Introduction
Welcome to this quirky, fun and important project where I will be comparing the performance of four important data structures: AVL Tree, Red-Black Tree, Chaining Hash Table, and Open Addressing Hash Table (double hashing). These data structures are widely used in various fields of computer science such as databases, file systems, and more. My goal in this project is to evaluate the time and space complexity of these structures and compare their performance.

# II. Background

Balanced trees and hash tables are two important data structures in computer science that are commonly used for efficient data storage and retrieval. 

A balanced tree is a data structure that maintains a balanced shape, ensuring that the height of the tree is proportional to the logarithm of the number of elements stored in it. This makes it possible to perform insertion, deletion, and search operations in O(log n) time, where n is the number of elements in the tree.

A hash table, on the other hand, is a data structure that uses a hash method to map keys to indices in an array. This allows for fast insertion, deletion, and search operations in O(1) time, on average, but it also uses a lot more space than a balanced tree.

Both of these data structure types have a worst-case time complexity of O(n), where n is the number of elements in the tree or hash table. This happens when the tree or hash table becomes unbalanced or when a collision occurs, respectively.

In this project I used AVL Tree, Red-Black Tree, Chaining Hash Table, and Open Addressing Hash Table (double hashing) as my data structures and implemented them in C++. These implementations aim to provide a balance between simplicity and performance, while somewhat adhering to standard programming practices and guidelines. In the following sections, I will describe my implementations in detail, including their technical details, algorithms, and design choices. I will also present my testing methodology and results, and discuss the strengths and weaknesses of each data structure.


# III. Technical Details
This is just a brief overview of the data structures. You can just skip straight to the [implementation](#iv-implementation) section if you want to see the code.
## A. AVL Tree
The avl tree is a self-balancing binary search tree. It is named after its two inventors, Georgy Adelson-Velsky and Evgenii Landis. The AVL tree was the first such data structure to be invented. In an AVL tree, the heights of the two child subtrees of any node differ by at most one; if at any time they differ by more than one, rebalancing is done to restore this property. Lookup, insertion, and deletion all take O(log n) time in both the average and worst cases, where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be rebalanced by one or more tree rotations.
Explaining the implementation of AVL tree is a bit complicated, so we will just give a link to a really good explanation of it: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/

## B. Red-Black Tree
Much like the AVL tree, the red-black tree is a self-balancing binary search tree. It is named after the color used to represent its two colors in its node representation. In a red-black tree, each node is colored either red or black. By constraining how unbalanced the tree can become in certain cases, the red-black tree ensures that the height of the tree remains O(log n) in the average case, where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be rebalanced by one or more tree rotations.
The red-black tree must conform to the following red-black properties in order to remain balanced:
1. Every node is either red or black.
2. The root is black.
3. Every leaf (NIL) is black (or in the case of this implementation, NULL)
4. If a node is red, then both its children are black.
5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.

## C. Chaining Hash Table
A hash table is a data structure that uses a hash method to map keys to indices in an array. This allows for fast insertion, deletion, and search operations in O(1) time, on average. In this implementation, we use chaining to resolve collisions. Chaining is a method of resolving collisions in which each index of the hash table's array is a pointer to a linked list. When a collision occurs, the new key-value pair is inserted into the linked list at the corresponding index. This allows for O(1) insertion, deletion, and search operations in the average case, but O(n) in the worst case, where n is the number of elements in the linked list at the corresponding index.

## D. Open Addressing Hash Table (double hashing)
Open addressing is a method of resolving collisions in which each index of the hash table's array is either empty or contains a key-value pair. When a collision occurs, the hash table probes the array for an empty slot using a probing method. If an empty slot is found, the new key-value pair is inserted into the array at that index. If no empty slot is found, the probing method is applied again to find a new index to probe. This process is repeated until an empty slot is found or the probing method returns an index that has already been probed. This allows for O(1) insertion, deletion, and search operations in the average case, but O(n) in the worst case, where n is the number of elements in the hash table.

# IV. Implementation
The implementation consists of a Testing/Benchmarking framework, the gui and the data structures themselves. The Testing/Benchmarking framework is used to test the performance of the data structures. The gui is used to visualize the data structures. The data structures themselves are implemented in the modules folder.
Let's start with the data structures themselves.
## Data Structures
### A. AVL Tree

Now we already know how the AVL tree works, so let's explore how it is implemented in the code. The implementation of the AVL tree is done in the [AVLTree.h](src/modules/AVLTree.h) and [AVLTree.cpp](src/modules/AVLTree.cpp) files. I recommend checking out the header file first, because it contains a quick documentation of the methods. The implementation of the methods is in the cpp file.

#### **AVL Node**
First we need a node struct to store the data in the tree. We have a deceleration of the AVLnode struct in [globals.h](src/globals.h#L45)
```cpp

struct AVLNode {
	int key; // key of the node
	int value; // value of the node
	AVLNode *parent; // parent is used for easier traversal and harder rotations. Idk why I went this route. Probably because I like pointers :D
	AVLNode *left; // left child
	AVLNode *right; // right child
	int count; // used for counting the number of nodes in the subtree
	int height; // height of the node
	AVLNode(); // default constructor
	AVLNode(int key); // constructor with key
	AVLNode(int key, int value); // constructor with key and value
};
```
The struct also has constructor for easier creation of nodes in [globals.cpp](src/globals.cpp#L4)

#### **AVL Variables**
```cpp
AVLNode *root; // root of the tree
int size; // number of nodes in the tree( mainly used to calculate the space complexity)
```

#### **AVL Helper methods**
The helper methods are used to get the height and balance of a node with the use of a ternary operator. If the node is NULL, the height or balance is 0, otherwise it is the height or balance of the node.  
[Helper methods](src/modules/AVLTree.cpp#L6)
```cpp
// These methods are used to get the height and balance of a node
int AVLTree::getHeight(AVLNode *node) { return (node == NULL) ? 0 : node->height; }
int AVLTree::getBalance(AVLNode *node) { return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right); }
```

#### **AVL Rotation**
The Rotations are handled in one method which takes the node to be rotated and the direction of the rotation as parameters. The method returns the new root of the subtree.

The RotateDirection enum is in [globals.h](src/globals.h#L24). Self explanatory.
```cpp
enum RotateDirection { LEFT, RIGHT };
```
When it comes to the rotations, they are implemented in one method because they are very similar and I dislike code duplication. The method takes the node to be rotated and the direction of the rotation as parameters.

Firstly it checks if the direction is left or right and then it prepares the nodes for the rotation. The nodes are `x` and `y`. `x` is the node to be rotated and `y` is the child of `x` in the direction of the rotation. After that it does the rotation and updates the heights of the nodes. The method returns the new root of the subtree.  
[AVLNode *AVLTree::rotate(AVLNode *x, RotateDirection direction)](src/modules/AVLTree.cpp#L9)
```cpp
AVLNode *AVLTree::rotate(AVLNode *x, RotateDirection direction) {
	AVLNode *y;
	// Prepare nodes
	if (direction == LEFT) {  // left rotation
		y = x->right;
		x->right = y->left;
		if (y->left != NULL) y->left->parent = x;

		y->left = x;
	} else {  // right rotation
		y = x->left;
		x->left = y->right;
		if (y->right != NULL) y->right->parent = x;

		y->right = x;
	}

	// Update parent
	y->parent = x->parent;
	x->parent = y;

	// Parent clause
	if (y->parent == NULL)
		root = y;
	else if (y->parent->left == x)
		y->parent->left = y;
	else
		y->parent->right = y;

	// Update heights
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));

	// Return new root
	return y;
}
```

#### **AVL Balance**
The balance method is used to balance the tree after an insertion or deletion. It receives the unbalanced node as its argument and than traverses the tree and balances the nodes. Its implemented iteratively since it is more efficient than recursion.

There is an alternative way to implement the different balance methods for insertion and deletion. But I found it to be unnecessary since this one, which is used for deletion, works for both insertion and deletion.  

I initially wanted to avoid describing the implementation details of the balance method, since you can easily refer to the code, however, I will go over it briefly. The balance method essentially prepares the necessary variables, updates the node height, checks the balance of the node, and performs appropriate rotations based on the specific case of imbalance. This process is repeated as it traverses up the tree. Admittedly, it may seem mundane, but it is crucial for maintaining the balance of the tree.

[AVLTree::balanceTree(AVLNode *node)](src/modules/AVLTree.cpp#L81)
```cpp
void AVLTree::balanceTree(AVLNode *node) {
	while (node != NULL) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);

		// Update height
		node->height = 1 + max(leftHeight, rightHeight);

		int balance = leftHeight - rightHeight;

		// If the node is unbalanced, then there are 4 cases
		//  1. if the node is left heavy and the left child is left heavy - right rotation
		//  2. if the node is left heavy and the left child is right heavy - left rotation followed by right rotation
		//  3. if the node is right heavy and the right child is right heavy - left rotation
		//  4. if the node is right heavy and the right child is left heavy - right rotation followed by left rotation
		if (balance > 1) {
			if (getBalance(node->left) >= 0)
				node = rotate(node, RIGHT);
			else {
				rotate(node->left, LEFT);
				node = rotate(node, RIGHT);
			}
		} else if (balance < -1) {
			if (getBalance(node->right) <= 0)
				node = rotate(node, LEFT);
			else {
				rotate(node->right, RIGHT);
				node = rotate(node, LEFT);
			}
		}

		node = node->parent;
	}
}
```
#### **AVL Insertion**
Insertion is a quite simple. To insert a node, the `insertNode` method takes in the key and value as parameters. If the node with the given key doesn't already exist, it creates a new node with the key and value, inserts it in the tree, and calls the balance method to ensure that the tree remains balanced. If the node already exists, it simply updates the value of the existing node.

Additionally, we have an insert method that takes in the key and value as parameters, creates a new node with the key and value, and calls the insertNode method. The insert method is mainly provided in case there's a need to pass in a node to be inserted instead of just the key and value. It also has a boolean balance parameter which is used to determine whether the tree should be balanced after the insertion. This is useful when inserting multiple nodes at once, since we can balance the tree after all the nodes have been inserted. Or just for testing purposes.

[AVLTree::insert(int key, int value)](src/modules/AVLTree.cpp#L146)  
[AVLTree::insertNode(AVLNode *node, bool balance)](src/modules/AVLTree.cpp#L148)
```cpp
void AVLTree::insert(int key, int value) { insertNode(new AVLNode(key, value), true); }

void AVLTree::insertNode(AVLNode *node, bool balance) {
	if (node == NULL) return;

	// If the tree is empty, make the node the root
	if (root == NULL) {
		root = node;
		size++;
		return;
	}

	// Find the appropriate parent for the node (make it a leaf)
	AVLNode *current = root;
	AVLNode *parent = NULL;
	while (current != NULL) {
		parent = current;
		// If the key is less than the current node's key, go to the left
		if (node->key < current->key) {
			current = current->left;
			continue;
		}
		// If the key already exists, increment the count and return
		if (node->key == current->key) {
			current->count++;
			current->value = node->value;
			delete node;
			return;
		}
		// Else, go to the right
		current = current->right;
	}

	// Insert the node depending on its key value
	if (node->key < parent->key)
		parent->left = node;
	else
		parent->right = node;

	// Set the parent
	node->parent = parent;

	// Update the size and count
	size++;
	node->count = 1;

	// Balance the tree
	if (balance)
		balanceTree(node->parent);
}
```

#### **AVL Search**
The search method is self explanatory. It searches for the node with the key and If it finds it, it returns the node, otherwise it returns NULL.
```cpp
AVLNode *AVLTree::searchKey(int key) {
	AVLNode *current = root;
	while (current != NULL) {
		if (current->key == key) return current;
		current = (key < current->key) ? current->left : current->right;
	}
	return NULL;
}
```

#### **AVL Deletion**
Deletion is a bit more complicated. To delete a node with a specific key, we use the `deleteKey` method, which searches the tree for the node and then calls the `deleteNode` method to remove it. If the node with the given key is not found, the `deleteNode` method checks that the node is not NULL before attempting to delete it.

[AVLTree::deleteKey(int key)](src/modules/AVLTree.cpp#L218)
```cpp
void AVLTree::deleteKey(int key) {
	AVLNode *node = searchKey(key);
	deleteNode(node);
}
```

The `deleteNode` method deletes the node and balances the tree. 
There are three cases:
 1. If the node has exactly one direct child, then the node is replaced by its child. The child's key and value are copied to the node and the child is deleted.
 2. If the node has two children, then the node is replaced by its successor. The successor is the node with the smallest key in the right subtree. The successor's key and value are copied to the node and the successor is deleted.
 3. If the node has no children, then it is simply deleted.
After the node is deleted, the tree is balanced.

*I used an xor fo the first time ever in this method. So that's fun*
```cpp
void AVLTree::deleteNode(AVLNode *node) {
	if (node == NULL) return;

	// If the node only has one direct child
	if ((node->right == NULL) ^ (node->left == NULL)) {
		// Set the child as the node's parent's left or right child
		AVLNode *child = (node->right == NULL) ? node->left : node->right;
		// If the node is the root, set the child as the new root
		// Otherwise, set the child as the node's parent's left or right child
		if (node->parent == NULL) {
			root = child;
			child->parent = NULL;
		} else if (node->parent->left == node) {
			node->parent->left = child;
			child->parent = node->parent;
		} else {
			node->parent->right = child;
			child->parent = node->parent;
		}
		size--;
		balanceTree(node->parent);
		delete node;
		return;
	}

	// If the node has both children
	// Here we don't delete the node, but we find the largest node in the left subtree and copy its key to the node to be "deleted"
	// After that, we delete the very node we copied the key from, because it has at most one child
	if (node->right != NULL && node->left != NULL) {
		// Find the largest node in the left subtree
		AVLNode *removeNode = node->left;
		while (removeNode->right != NULL)
			removeNode = removeNode->right;

		// Copy the largest subtree's node's key to the node to be deleted
		node->key = removeNode->key;
		node->value = removeNode->value;

		// Make a pointer to the node to be deleted's parent
		AVLNode *tempLeafNode;
		tempLeafNode = removeNode->parent;

		// Delete the largest node
		deleteNode(removeNode);

		// Use the tempLeafNode pointer to balance the tree
		balanceTree(tempLeafNode);

		return;
	}

	// === If the node is a leaf ===
	size--;

	// if the node is the root just remove it
	if (node->parent == NULL) {
		root = NULL;
		delete node;
		return;
	}

	// Otherwise find which child of the parent is the node and set it to null
	if (node->parent->left == node)
		node->parent->left = NULL;
	else
		node->parent->right = NULL;

	balanceTree(node->parent);
	delete node;
}
```

### B. Red-Black Tree
Same as in the AVL Tree section, the Red-Black Tree is implemented in [RedBlackTree](src/modules/RedBlackTree.cpp) and [RedBlackTree.h](src/modules/RedBlackTree.h).
The main difference between the theoretical implementation of the Red-Black Tree and my implementation is that I didn't use NIL nodes to represent the leaves of the tree. The reason is less memory usage. although it does somewhat backfire when implementing some methods. So yeah, Fun.
#### **Red Black Node**
The node structure is identical to the AVL Tree node structure, except that it has a color field instead of a height field. 
[RedBlackNode structure](src/globals.h#L65)
```cpp
// RedBlackNode structure
struct RedBlackNode {
	int key; // The key of the node
	int value; // The value of the node
	RedBlackNode *parent; // The parent of the node
	RedBlackNode *left; // The left child of the node
	RedBlackNode *right; // The right child of the node
	int count; // The number of times the node has been inserted
	RedBlackNodeColor color;
	RedBlackNode(); // Default constructor
	RedBlackNode(int key, int value); // Key-Value Constructor
	RedBlackNode(int key, int value, RedBlackNodeColor color); // Key-Value-Color Constructor
};
```
The color field is an enum with two values: `black` and `red`.

[RedBlackNodeColor enum](src/globals.h#L59)
```cpp
enum RedBlackNodeColor {
	black,
	red
};
```
#### **Red Black Variables**
The Red-Black Tree has exactly the same variables as the AVL Tree.
```cpp
RedBlackNode *root; // The root of the tree
int size; // The number of nodes in the tree
```
#### **Red Black Helper Functions**
here we have more helper methods than in the AVL Tree since Red-Black Trees are more complicated.

* Rotate  
The rotate method identical to the [AVL Tree rotate method](#avl-rotation), except that it doesn't update the height of the nodes.  
[RedBlackNode *RedBlackTree::rotate(RedBlackNode *x, RotateDirection direction](src/modules/RedBlackTree.cpp#L38)
* The transplant method simply replaces the subtree rooted at node u with the subtree rooted at node v.  
[void RedBlackTree::transplant(RedBlackNode *u, RedBlackNode *v) ](src/modules/RedBlackTree.cpp#L71)
	```cpp
	void RedBlackTree::transplant(RedBlackNode *u, RedBlackNode *v) {
		// If v is not null, set its parent to u's parent
		if (v != NULL)
			v->parent = u->parent;

		// If the parent of u is null, set the root to v
		if (u->parent == NULL) {
			root = v;
			return;
		}
		// If u is the left child of its parent, set the left child of the parent to v
		// Otherwise, set the right child of the parent to v
		(u == u->parent->left) ? u->parent->left = v : u->parent->right = v;
	}
	```
* The successor method returns the node with the smallest key that is larger than the key of the given node.
[RedBlackNode *RedBlackTree::successor(RedBlackNode *node)](src/modules/RedBlackTree.cpp#L86)
	```cpp
	RedBlackNode *RedBlackTree::successor(RedBlackNode *node) {
		// If the right child of the node is not null, return the leftmost child of the right child
		if (node->right != nullptr) {
			node = node->right;
			while (node->left != nullptr)
				node = node->left;

			return node;
		}
		// Otherwise, return the first ancestor of the node that is a left child of its parent
		RedBlackNode *parent = node->parent;
		while (parent != nullptr && node == parent->right) {
			node = parent;
			parent = parent->parent;
		}
		return parent;
	}
	```
#### **Red Black Insertion**
Insertion consists of two parts: the insertion of the node and the fixing(balancing) of the tree.

The insertion part, is similar to AVL Tree.
Here I also require the key and value as parameters. for the `insert` method. If there is not root then I set it as root and color it black. If not than I search for its new position and consecutively if it already exists. In the case it already exists I increment count and exit. Otherwise I set the new parent for the new node and update that parents left/right child reference. Finally we check if `fixInsert` needs to be called.
[void RedBlackTree::insert(int key, int value)](src/modules/RedBlackTree.cpp#L139)
```cpp
void RedBlackTree::insert(int key, int value) {
	insertNode(new RedBlackNode(key, value));
}

void RedBlackTree::insertNode(RedBlackNode *node) {
	if (root == NULL) {
		root = node;
		size++;
		root->color = black;
		return;
	}

	// Initialize current and parent
	RedBlackNode *current = this->root;
	RedBlackNode *parent = NULL;  // Future parent of the node

	// Find the parent of the node
	while (current != NULL) {
		parent = current;
		if (node->key == current->key) {
			current->count++;
			delete node;
			return;
		}
		current = (node->key < current->key) ? current->left : current->right;
	}

	// Set the parent of the node
	node->parent = parent;
	// Set the node as the left or right child of the parent
	(node->key < parent->key) ? parent->left = node
							  : parent->right = node;
	size++;

	// If the parent is the root, the tree is balanced
	if (node->parent == NULL) {
		node->color = black;
		return;
	}

	// If the parent is black, or the grandparent is NULL, the tree is balanced
	if (node->parent->color == black || node->parent->parent == NULL) return;

	// Balance the tree
	fixInsertFrom(node);
}
```
To fix the red black node distribution so that is conforms to the rules of the Red Black Tree we use the `fixInsertFrom` method. It takes a pointer to the newly inserted node as a parameter.

The method loops until the parent of the newly inserted node is black. If the parent is red, it checks whether the uncle of the newly inserted node is red or black. If the uncle node is red (Case 1), it recolors the parent, the uncle, and the grandparent to black, black, and red, respectively. Then, the newly inserted node is set to its grandparent and the loop continues.

If the uncle node is black (Case 2), it checks whether the newly inserted node is on the same side as its parent or on the opposite side. If it's on the opposite side, a rotation is performed to move the newly inserted node to the same side as its parent. Then, it recolors the parent and grandparent to black and red, respectively, and performs a rotation on the grandparent.

The method continues until the root node is reached or a black parent is encountered. Finally, the color of the root node is set to black.

[void RedBlackTree::fixInsertFrom(RedBlackNode *node)](src/modules/RedBlackTree.cpp#L104)
```cpp
void RedBlackTree::fixInsertFrom(RedBlackNode *node) {
	RedBlackNode *parent;
	// Continue looping until parent node is black
	while (node->parent != NULL && node->parent->color == red) {
		parent = node->parent;
		// Determine which side of the grandparent node the parent is on
		bool parentIsLeft = (parent == parent->parent->left);
		RedBlackNode *uncle = parentIsLeft ? parent->parent->right : parent->parent->left;
		// Case 1: Uncle node is red
		if (uncle != NULL && uncle->color == red) {
			parent->color = black;
			uncle->color = black;
			parent->parent->color = red;
			node = parent->parent;
		}
		// Case 2: Uncle node is black
		else {
			// Determine whether node is on the same side as the parent or opposite side
			bool nodeIsLeft = (node == (parentIsLeft ? parent->left : parent->right));
			// If node is on the opposite side of the parent, rotate node up to be on the same side
			if (!nodeIsLeft) {
				node = parent;
				rotate(node, parentIsLeft ? LEFT : RIGHT);
				parent = node->parent;
			}
			// Recolor parent and grandparent, and rotate grandparent
			parent->color = black;
			parent->parent->color = red;
			rotate(parent->parent, parentIsLeft ? RIGHT : LEFT);
		}
		if (node == root) break;
	}
	root->color = black;
}
```
#### **Red Black Search**
Red Black search is exactly same as in AVL. So i'm just gonna link that section to avoid repeating myself. [AVL Search](#avl-search)

#### **Red Black Delete**
Just like in insertion, deletion consists of two parts: the deletion of the node and the fixing(balancing) of the tree.

In the `deleteNode` method we do stuff. bla bla bla TODO: Continue

[void RedBlackTree::deleteKey(int key)](src/modules/RedBlackTree.cpp#L198)
```cpp
void RedBlackTree::deleteNode(RedBlackNode *node) {
	if (node == nullptr) return;

	// Determine the node to delete (y) and its replacement (x)
	RedBlackNode *y = (node->left == nullptr || node->right == nullptr) ? node : successor(node);
	RedBlackNode *x = (y->left != nullptr) ? y->left : y->right;

	// If x is null, create a dummy NIL node
	if (x == nullptr) x = new RedBlackNode(-1, -1, black);

	// Set the parent of x to the parent of y
	x->parent = y->parent;

	// Replace y with x in the tree
	if (y->parent == nullptr)
		root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	// If y and node are different nodes, copy the values from y to node
	if (y != node) {
		node->key = y->key;
		node->value = y->value;
		node->count = y->count;
	}

	// If y was black, fix the tree starting from x
	if (y->color == black) fixDeleteFrom(x);

	// If x is a dummy NIL node, delete it
	if (x->key == -1 && x->value == -1) {
		if (x->parent == nullptr)  // If x is the root, set the root to null
			root = nullptr;
		else if (x == x->parent->left)	// If x is a left child, set the left child to null
			x->parent->left = nullptr;
		else  // If x is a right child, set the right child to null
			x->parent->right = nullptr;

		delete x;
	}
	size--;
	// Delete y
	delete y;
}
```

### C. Chaining Hash Table
### D. Open Addressing Hash Table (double hashing)
## Testing/Benchmarking framework
# V. Testing
# VI. Conclusion


