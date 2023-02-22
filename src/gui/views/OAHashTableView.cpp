#include "OAHashTableView.h"
#include <gui.h>
#include <imgui.h>
#include <cstdlib>
#define IG GUI::imgui

static OpenAdressingHashTable table = OpenAdressingHashTable(100, new int[11]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1}, new int[11]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1});
// OpenAdressingHashTable table = OpenAdressingHashTable(10);
static int insertKey = 0;
static int insertValue = 0;
static int highlightIndex = -1;
// timer
static int timer = 0;
void OAHashTableView::draw() {
	IG::Begin("Add Entry");
	IG::InputInt("Key", &insertKey);
	IG::InputInt("Value", &insertValue);
	if (IG::Button("Add Entry"))
		table.insertKey(insertKey, insertValue);
	IG::End();

	IG::Begin("Create Random Table");
	static int amount = 10;
	static int tableSize = 10;
	IG::InputInt("Amount", &amount);
	IG::InputInt("Size", &tableSize);
	if (IG::Button("Create Table")) {
		// Destroy the old table
		table.~OpenAdressingHashTable();
		// Create a new table
		table = OpenAdressingHashTable(tableSize);
		for (int i = 0; i < amount; i++)
			table.insertKey(rand() % 100, rand() % 100);
	}
	IG::End();

	IG::Begin("Search Entry");
	static int searchKey = 0;
	IG::InputInt("Key", &searchKey);
	if (IG::Button("Search Entry")) {
		highlightIndex = table.getIndex(searchKey);
	}
	// Draw text notifying the user that the key was not found
	if (timer > 0 && highlightIndex == -1) {
		IG::Text("Key not found");
		timer--;
	}
	IG::End();

	// Delete

	IG::Begin("Delete Entry");
	static int deleteKey = 0;
	IG::InputInt("Key", &deleteKey);
	if (IG::Button("Delete Entry"))
		table.deleteKey(deleteKey);
	IG::End();

	drawTable();
	drawTableWindow();
}

static void drawTableWindow() {
	IG::Begin("Table");
	IG::Text("Key");
	IG::SameLine();
	IG::Text("Value");
	for (int i = 0; i < table.capacity; i++) {
		IG::Text("%d", table.table[i].key);
		IG::SameLine();
		IG::Text("%d", table.table[i].value);
		// Add separator
		if (i != table.capacity - 1) IG::Separator();
	}
	IG::End();
}

static void drawTable() {
	// Draw the table relations on the frame using rect
	int rectSize = 25;
	int spacing = 50;

	// Calculate amount of rows and columns (square)
	int rows = 1;
	int columns = 1;
	while (rows * columns < table.capacity) (rows > columns) ? columns++ : rows++;

	int x = GUI::getWidth() / 2 - ((rectSize + spacing) * columns) / 2;
	int y = 100;
	// push x to the left depending on the table size
	GUI::beginMain();
	for (int i = 0; i < table.capacity; i++) {
		// Draw the rect
		if (table.table[i].value != -1) {
			if (i == highlightIndex)
				GUI::rect(x, y, rectSize, rectSize, new int[3]{255, 0, 0});
			else
				GUI::rect(x, y, rectSize, rectSize, new int[3]{255, 255, 255});

			// Draw the text
			GUI::text(x, y, std::to_string(table.table[i].key).c_str(), new int[3]{0, 0, 0});
		} else
			GUI::rect(x, y, rectSize, rectSize, new int[3]{0, 0, 0});

		// Draw the line
		if (i != table.capacity - 1) {
			// Draw the line
			GUI::line(x + rectSize, y + rectSize / 2, x + rectSize + spacing, y + rectSize / 2, new int[3]{255, 255, 255});
			// Draw the arrow
			GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 - 10, new int[3]{255, 255, 255});
			GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 + 10, new int[3]{255, 255, 255});
		}
		// Draw the highlight

		// Move to the next column
		if ((i + 1) % columns == 0) {
			x = GUI::getWidth() / 2 - ((rectSize + spacing) * columns) / 2;
			y += rectSize + spacing;
		} else
			x += rectSize + spacing;
	}
	IG::End();
}