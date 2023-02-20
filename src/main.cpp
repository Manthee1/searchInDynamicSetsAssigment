// Your task in this assignment is to implement and then compare 4
// implementations of data structures in terms of the effectiveness of insert,
// delete and search operations in different situations:
//
// - (3 points) Implementation of a binary search tree (BVS) with any balancing
// algorithm, e.g. AVL, Red-Black Trees, (2,3) Trees, (2,3,4) Trees, Splay
// Trees, ...
// - (3 points) Second implementation of BVS with a different balancing
// algorithm than in the previous point.
// - (3 points) Implementation of a hash table with collision resolution of your
// choice. The hash table size adjustment must also be implemented.
// - (3 points) Second implementation of the hash table with collision
// resolution in a different way than in the previous point. The hash table size
// adjustment must also be implemented.

#include <iostream>
#include <sstream>
#include "gui.h"
#include "tests/test.h"
#include "modules/AVLTree.h"
#include "gui/imgui.h"
#include "gui/views/AVLTreeUI.h"
using namespace std;

#define IG GUI::imgui

struct ViewData {
	string name;
	void (*draw)();
};

ViewData views[] = {
	{"AVL", AVLTreeUI::draw},
	{"RedBlack", NULL},
	{"Splay", NULL},
	{"Hash", NULL},
};
int activeView = 0;

void drawDashboard() {
	// Draw a docked window to the left of the screen
	IG::SetNextWindowPos(ImVec2(0, 0));
	IG::SetNextWindowCollapsed(false, ImGuiCond_Once);
	IG::SetNextWindowSize(ImVec2(200, IG::GetIO().DisplaySize.y));

	IG::Begin("Dashboard");
	// Create a button for each view, substituting it with text if the view is active
	for (int i = 0; i < 4; i++) {
		if (i == activeView) {
			IG::Text(views[i].name.c_str());
			continue;
		}
		if (IG::Button(views[i].name.c_str()))
			activeView = i;
	}

	IG::End();
}
void draw() {
	// Draw a circle
	// GUI::circle(100.0f, 100.0f, 50.0f, new int[3]{255, 0, 0});
	// GUI::line(100.0f, 100.0f, 200.0f, 200.0f, new int[3]{0, 255, 0});
	int width = IG::GetIO().DisplaySize.x;
	int height = IG::GetIO().DisplaySize.y;

	drawDashboard();
	// Draw the tree
	void (*drawFunc)() = views[activeView].draw;
	if (drawFunc != NULL)
		drawFunc();
}

// Add --test to the command line to run the tests
int main(int argc, char** argv) {
	if (argc > 1) {
		string arg = string(argv[1]);

		if (arg == "--test") {
			testAVLTree();
			// testAVLTree2();
			// testHashTable();
			// testHashTable2();
			return 0;
		}
		return 0;
	}
	GUI::init();
	while (!GUI::windowShouldClose()) GUI::render(draw);
	GUI::quit();

	return 0;
}