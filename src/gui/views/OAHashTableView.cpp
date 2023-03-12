#include "OAHashTableView.h"
#include <gui.h>
#include <imgui.h>
#include <cstdlib>
#define IG GUI::imgui

// static OAHashTable table;
static OAHashTable* table;
static char* insertKeyCharArray = new char[100]{"\0"};
static std::string insertKey;
static int insertValue = 0;
static int highlightIndex = -1;
// timer
static int timer = 0;
void OAHashTableView::draw() {
	IG::Begin("Add Entry");
	IG::InputText("Key", insertKeyCharArray, 100);
	IG::InputInt("Value", &insertValue);
	if (IG::Button("Add Entry")) {
		table->insertKey(insertKeyCharArray, insertValue);
	}
	IG::End();

	IG::Begin("Create Random Table");
	static int amount = 10;
	IG::InputInt("Amount", &amount);
	if (IG::Button("Create Table")) {
		// Create a new table
		table = new OAHashTable(amount * 2);
		for (int i = 0; i < amount; i++)
			table->insertKey(generateRandomString(5), rand() % 100);
	}
	IG::End();

	IG::Begin("Search Entry");
	static char* searchKey = new char[100]{"\0"};
	IG::InputText("Key", searchKey, 100);
	if (IG::Button("Search Entry")) {
		highlightIndex = table->getKeyIndex(searchKey);
	}
	// Draw text notifying the user that the key was not found
	if (timer > 0 && highlightIndex == -1) {
		IG::Text("Key not found");
		timer--;
	}
	IG::End();

	// Delete

	IG::Begin("Delete Entry");
	static char* deleteKey = new char[100]{"\0"};
	IG::InputText("Key", deleteKey, 100);
	if (IG::Button("Delete Entry"))
		table->deleteKey(deleteKey);
	IG::End();

	drawTable();
	drawTableWindow();
}

static void drawTableWindow() {
	if (table == NULL) return;
	IG::Begin("Table");
	IG::Text("Key");
	IG::SameLine();
	IG::Text("Value");
	for (int i = 0; i < table->capacity; i++) {
		if (table->table->keys[i] == NULL) continue;
		IG::Text("%s", (table->table->keys[i])->c_str());
		IG::SameLine();
		IG::Text("%d", table->table->values[i]);
		// Add separator
		if (i != table->capacity - 1) IG::Separator();
	}
	IG::End();
}

static void drawTable() {
	if (table == NULL) return;
	// Draw the table relations on the frame using rect
	int rectSize = 25;
	int spacing = 50;

	// Calculate amount of rows and columns (square)
	int rows = 1;
	int columns = 1;
	while (rows * columns < table->capacity) (rows > columns) ? columns++ : rows++;

	int x = GUI::getWidth() / 2 - ((rectSize + spacing) * columns) / 2;
	int y = 100;
	// push x to the left depending on the table size
	GUI::beginMain();

	// COlors
	int colorWhite[3] = {255, 255, 255};
	int colorBlack[3] = {0, 0, 0};
	int colorRed[3] = {255, 0, 0};
	for (int i = 0; i < rows; i++) {
		y += rectSize + spacing;
		if (GUI::isLowerThanScreen(y + rectSize))
			continue;
		if (GUI::isHigherThanScreen(y))
			break;
		x = GUI::getWidth() / 2 - ((rectSize + spacing) * columns) / 2;
		for (int j = 0; j < columns; j++) {
			x += rectSize + spacing;
			if (GUI::isLeftOfScreen(x + rectSize + spacing))
				continue;
			if (GUI::isRightOfScreen(x))
				break;
			int index = i * columns + j;

			// Draw the rect
			if (table->table->keys[index] != NULL) {
				GUI::rect(x, y, rectSize, rectSize, (index == highlightIndex) ? colorRed : colorWhite);

				// Draw the index, key and value
				GUI::text(x, y, std::to_string(index), colorBlack);
				GUI::text(x, y + 10, *table->table->keys[index], colorBlack);
				GUI::text(x, y + 20, std::to_string(table->table->values[index]), colorBlack);
			} else {
				GUI::rect(x, y, rectSize, rectSize, colorBlack);
				// Index
				GUI::text(x, y, std::to_string(index), colorWhite);
			}

			// Draw the line
			if (index != table->capacity - 1) {
				// Draw the line
				GUI::line(x + rectSize, y + rectSize / 2, x + rectSize + spacing, y + rectSize / 2, colorWhite);
				// Draw the arrow
				GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 - 10, colorWhite);
				GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 + 10, colorWhite);
			}
			// Draw the highlight
		}
	}
	IG::End();
}