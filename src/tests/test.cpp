#include "../utils.cpp"
#include "../globals.cpp"
#include "../modules/AVLTree.cpp"
#include <array>
using namespace std;

Node* codeToTree(string code);
bool compareTrees(Node* tree1, Node* tree2);

InsertionTestEntry* AVLInsertionTests = new InsertionTestEntry[10]{
	{new int[3]{1, 2, 3}, 3, "2/1-\\3", "Simple left rotate test"},
	{new int[3]{3, 2, 1}, 3, "2/1-\\3", "Simple right rotate test"},
	{new int[3]{1, 3, 2}, 3, "2/1-\\3", "Left-right rotate test"},
	{new int[3]{3, 1, 2}, 3, "2/1-\\3", "Right-left rotate test"},
	// More complex tests
	{new int[5]{1, 2, 3, 4, 5}, 5, "2/1-\\4/3-\\5", "Complex test 1"},
	{new int[5]{5, 4, 3, 2, 1}, 5, "4/2/1-\\3--\\5", "Complex test 2"},
	{new int[7]{5, 8, 7, 4, 7, 1, 3}, 7, "7/4/1\\3--\\5--\\8/7", "Complex test 3"},
	{new int[7]{1, 9, 4, 7, 3, 2, 6}, 7, "4/2/1-\\3--\\7/6-\\9", "Complex test 4"},
	{new int[8]{1, 5, 7, 3, 2, 4, 6, 8}, 8, "3/2/1--\\5/4-\\7/6-\\8", "Complex test 5"},
	{new int[31]{7, 1, 2, 9, 5, 3, 5, 1, 2, 6, 3, 3, 1, 1, 7, 2, 4, 5, 6, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 31, "5/2/1/1/0--\\1/1-\\1---\\3/2/2-\\2--\\3/3-\\4/3-\\4-----\\7/6/5/5-\\5--\\6\\6---\\8/7\\7--\\9/8-\\9\\9", "Insane test"},
};

DeletionTestEntry* AVLDeletionTests = new DeletionTestEntry[15]{
	{new int[3]{1, 2, 3}, 3, new int[1]{2}, 1, "1\\3", "Root node deletion test"},
	{new int[3]{3, 2, 1}, 3, new int[1]{3}, 1, "2/1", "Right child node with no children deletion test"},
	{new int[4]{3, 2, 1, 4}, 4, new int[1]{3}, 1, "2/1-\\4", "Right child node with a right child deletion test"},
	{new int[4]{1, 2, 4, 3}, 4, new int[1]{4}, 1, "2/1-\\3", "Right child node with a left child deletion test"},
	{new int[5]{1, 2, 3, 4, 5}, 5, new int[1]{4}, 1, "2/1-\\3\\5", "Right child node with both children deletion test"},
	{new int[3]{1, 2, 3}, 3, new int[1]{1}, 1, "2\\3", "Left child node with no children deletion test"},
	{new int[4]{5, 2, 6, 3}, 4, new int[1]{2}, 1, "5/3-\\6", "Left child node with a right child deletion test"},
	{new int[4]{4, 2, 3, 1}, 4, new int[1]{2}, 1, "3/1-\\4", "Left child node with a left child deletion test"},
	{new int[5]{5, 2, 6, 3, 1}, 5, new int[1]{2}, 1, "5/1\\3--\\6", "Left child node with both children deletion test"},

	// More complex tests
	{new int[7]{5, 8, 7, 4, 7, 1, 3}, 7, new int[2]{4, 7}, 2, "5/3/1--\\8/7", "Complex test 1"},
	{new int[7]{1, 9, 4, 7, 3, 2, 6}, 7, new int[3]{4, 3, 2}, 3, "7/1\\6--\\9", "Complex test 2"},	// TODO: Fails on last delete, The key goes completely whacko
	{new int[8]{1, 5, 7, 3, 2, 4, 6, 8}, 8, new int[4]{3, 2, 1, 5}, 4, "7/4/6\\8", "Complex test 3"},
	{new int[31]{7, 1, 2, 9, 5, 3, 5, 1, 2, 6, 3, 3, 1, 1, 7, 2, 4, 5, 6, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 31, new int[31]{5, 5, 9, 1, 2, 3, 4, 6, 8, 1, 3}, 31, "4/1/1/0-\\1--\\2/2/2--\\3\\3----\\7/5/5-\\6\\6---\\7/7-\\9/8-\\9", "Insane test"},
};

AVLTree createTree(int keys[], int length) {
	AVLTree tree = AVLTree();
	for (int i = 0; i < length; i++)
		tree.insertKey(keys[i]);

	return tree;
}

bool testAVLTreeInsertion() {
	int failedTests = 0;
	cout << "Testing AVL tree insertion..." << endl;
	for (int i = 0; i < 10; i++) {
		cout << "Test " << i + 1 << " [" << AVLInsertionTests[i].name << "]: ";
		Node* treeRoot = createTree(AVLInsertionTests[i].keys, AVLInsertionTests[i].length).root;
		Node* codeRoot = codeToTree(AVLInsertionTests[i].code);
		if (compareTrees(treeRoot, codeRoot)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		printf(RED "Failed\n" RESET);
		printf("Expected:\n");
		printTree(codeRoot, 0);
		printf("Got:\n");
		printTree(treeRoot, 0);

		failedTests++;
	}

	cout << "AVL tree insertion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTreeDeletion() {
	int failedTests = 0;
	cout << "Testing AVL tree deletion..." << endl;
	for (int i = 0; i < 15; i++) {
		cout << "Test " << i + 1 << " [" << AVLDeletionTests[i].name << "]: ";
		AVLTree tree = createTree(AVLDeletionTests[i].keys, AVLDeletionTests[i].length);
		for (int j = 0; j < length(AVLDeletionTests[i].deleteKeys); j++)
			tree.deleteKey(AVLDeletionTests[i].deleteKeys[j]);

		Node* codeRoot = codeToTree(AVLDeletionTests[i].code);
		if (compareTrees(tree.root, codeRoot)) {
			printf(GREEN "Passed\n" RESET);
			continue;
		}

		printf(RED "Failed\n" RESET);
		printf("Expected:\n");
		printTree(codeRoot, 0);
		printf("Got:\n");
		printTree(tree.root, 0);

		failedTests++;
	}

	cout << "AVL tree deletion test finished with " << failedTests << " failed tests" << endl;

	return failedTests == 0;
}

bool testAVLTree() {
	if (!testAVLTreeInsertion()) {
		cout << "AVL Tree insertion failed!" << endl;
		cout << "Will not test deletion" << endl;
		return false;
	}
	cout << "AVL Tree insertion passed!" << endl;

	if (!testAVLTreeDeletion()) {
		cout << "AVL Tree deletion failed!" << endl;
		return false;
	}
	cout << "AVL Tree deletion passed!" << endl;

	return true;
}

Node* codeToTree(string code) {
	// The first character is the root node
	Node* root = createNode(code[0] - '0');

	// The rest of the characters are the children
	//  '/' means right node, '\' means left node
	Node* current = root;
	for (int i = 0; i < (int)code.length(); i++) {
		if (code[i] == '-') {
			current = current->parent;
			continue;
		}
		if (code[i] == '\\') {
			current->right = createNode(code[++i] - '0');
			current->right->parent = current;
			current = current->right;
		} else if (code[i] == '/') {
			current->left = createNode(code[++i] - '0');
			current->left->parent = current;
			current = current->left;
		}
	}
	return root;
}

bool compareTrees(Node* tree1, Node* tree2) {
	if (tree1 == NULL && tree2 == NULL) return true;
	if (tree1 == NULL || tree2 == NULL) return false;
	if (tree1->key != tree2->key) return false;
	return compareTrees(tree1->left, tree2->left) && compareTrees(tree1->right, tree2->right);
}