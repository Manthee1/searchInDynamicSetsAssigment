#include "ViewController.h"

#include "./lib/gui.h"
#include "./views/AVLTreeView.h"
#include "./views/CHashTableView.h"
#include "./views/OAHashTableView.h"
#include "./views/RedBlackTreeView.h"

#define IG GUI::imgui

int ViewController::currentView = 0;

ViewData ViewController::views[] = {
	{"AVL", AVLTreeView::draw},
	{"RedBlack", RedBlackTreeView::draw},
	{"HashTable (Chaining)", CHashTableView::draw},
	{"HashTable (OpenAddressing)", OAHashTableView::draw}};

void drawDashboard() {
	// Draw a docked window to the left of the screen
	IG::SetNextWindowPos(ImVec2(0, 0));
	IG::SetNextWindowCollapsed(false, ImGuiCond_Once);
	IG::SetNextWindowSize(ImVec2(200, IG::GetIO().DisplaySize.y));

	IG::Begin("Dashboard");
	// Create a button for each view, substituting it with text if the view is active
	for (int i = 0; i < 4; i++) {
		if (i == ViewController::currentView) {
			IG::Text(ViewController::views[i].name.c_str());
			continue;
		}
		if (IG::Button(ViewController::views[i].name.c_str()))
			ViewController::currentView = i;
	}

	IG::End();
}
void ViewController::draw() {
	// Draw a circle
	// GUI::circle(100.0f, 100.0f, 50.0f, new int[3]{255, 0, 0});
	// GUI::line(100.0f, 100.0f, 200.0f, 200.0f, new int[3]{0, 255, 0});
	int width = IG::GetIO().DisplaySize.x;
	int height = IG::GetIO().DisplaySize.y;

	drawDashboard();
	// Draw the tree
	void (*drawFunc)() = views[ViewController::currentView].draw;
	if (drawFunc != NULL)
		drawFunc();
}

void ViewController::run() {
	GUI::init();
	while (!GUI::windowShouldClose()) GUI::render(draw);
	GUI::quit();
}

void ViewController::run(ViewType viewType) {
	ViewController::currentView = viewType;
	run();
}