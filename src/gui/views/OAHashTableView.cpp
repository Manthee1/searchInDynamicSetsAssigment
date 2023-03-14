#include "OAHashTableView.h"
#include <gui.h>
#include <imgui.h>
#include <cstdlib>
#define IG GUI::imgui

#define RECT_SIZE 25
#define SPACING 50
#define START_X 200
#define START_Y 200

// static OAHashTable table;
static OAHashTable* table = new OAHashTable(1);
static std::string insertKey;
static int insertValue = 0;
static std::string selectedKey = "";
static int selectedValue = 0;
static int selectedIndex = -1;
static int changeValue = 0;

static int rows = 0;
static int columns = 0;
static int size = 0;
static void updateTableDimensions() {
	rows = 1;
	columns = 1;
	while (rows * columns < table->capacity) (rows > columns) ? columns++ : rows++;
}

void OAHashTableView::draw() {
	IG::Begin("Open Addressing Hash Table");

	IG::Text("Table Size: %d", table->size);
	IG::Text("Table Capacity: %d", table->capacity);
	IG::Text("Tombstone Count: %d", table->tombstoneCount);
	IG::Text("Load Factor: %f", (float)table->size / (float)table->capacity);
	IG::Text("Tombstone Factor: %f", (float)table->tombstoneCount / (float)table->capacity);

	IG::Separator();

	IG::Text("Create Random Table");
	static int amount = 10;
	IG::InputInt("Amount", &amount);
	if (IG::Button("Create Table")) {
		// Delete the old table
		delete table;
		// Create a new table
		table = new OAHashTable(amount);
		for (int i = 0; i < amount; i++)
			table->insertKey(generateRandomString(5), rand() % 100);
	}

	IG::Separator();

	static char* insertKeyCharArray = new char[100]{"\0"};
	IG::Text("Add Entry");
	IG::InputText("Add Key", insertKeyCharArray, 100);
	IG::InputInt("Value", &insertValue);
	if (IG::Button("Add Entry"))
		table->insertKey(insertKeyCharArray, insertValue);

	IG::Separator();

	IG::Text("Search Entry");
	static char* searchKey = new char[100]{"\0"};
	IG::InputText("Search Key", searchKey, 100);
	if (IG::Button("Search Entry")) {
		int newSelectedValue = table->searchKey(searchKey);
		if (newSelectedValue != -1) {
			selectedKey = searchKey;
			selectedValue = newSelectedValue;
		}
	}

	IG::Separator();

	// Delete
	IG::Text("Delete Entry");
	static char* deleteKey = new char[100]{"\0"};
	IG::InputText("Delete Key", deleteKey, 100);
	if (IG::Button("Delete Entry"))
		table->deleteKey(deleteKey);
	if (IG::Button("Clear Table")) {
		delete table;
		table = NULL;
	}

	IG::Separator();

	IG::Text("Other Functions");
	// Rehash
	if (IG::Button("Rehash"))
		table->rehash();

	IG::End();

	// Check if mouse clicked on a table entry
	if (IG::IsMouseClicked(0) && table != NULL) {
		// Get mouse position
		int mouseX = GUI::getMouseX();
		int mouseY = GUI::getMouseY();
		// Check if mouse is inside the table
		if (mouseX > START_X && mouseX < START_X + columns * (RECT_SIZE + SPACING) &&
			mouseY > START_Y && mouseY < START_Y + rows * (RECT_SIZE + SPACING)) {
			int clickNode = (mouseX - START_X) / (RECT_SIZE) + (mouseY - START_Y) / (RECT_SIZE)*columns;

			if (clickNode % 3 == 0 && table->table->keys[clickNode / 3] != NULL) {
				clickNode /= 3;
				selectedKey = *(table->table->keys[clickNode]);
				selectedValue = table->table->values[clickNode];
				selectedIndex = clickNode;
				changeValue = selectedValue;
			}
		}
	}

	if (selectedKey != "") {
		IG::Begin("Selected Entry");
		IG::Text("Index: %d", selectedIndex);
		IG::Text("Key: %s", selectedKey.c_str());
		IG::Text("Value: %d", selectedValue);
		if (IG::Button("Clear Selection")) {
			selectedKey = "";
			selectedValue = 0;
		}

		IG::Separator();

		// Change value
		IG::Text("Change Value");
		IG::InputInt("Change", &changeValue);
		if (IG::Button("Change Value"))
			table->insertKey(selectedKey, changeValue);

		IG::Separator();

		// Delete
		if (IG::Button("Delete Entry")) {
			table->deleteKey(selectedKey);
			selectedKey = "";
			selectedValue = 0;
		}

		// Focus on the node
		if (IG::Button("Focus on Node")) {
			GUI::focusOn(START_X + (selectedIndex % columns) * (RECT_SIZE + SPACING) + RECT_SIZE / 2,
						 START_Y + (selectedIndex / columns) * (RECT_SIZE + SPACING) + RECT_SIZE / 2, 2);
		}

		IG::End();
	}
	drawTable();
	drawTableWindow();
}

static void drawTableWindow() {
	if (table == NULL) return;
	IG::Begin("Table");
	IG::Text("Key");
	IG::SameLine();
	IG::Text("Value");
	// Loop through the table and draw the keys and values
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
	int rectSize = RECT_SIZE;
	int spacing = SPACING;

	// Calculate amount of rows and columns (square)
	if (size != table->capacity) {
		updateTableDimensions();
		size = table->capacity;
	}

	// subtract the spacing from the start position
	int x = START_X - rectSize - spacing;
	int y = START_Y - rectSize - spacing;
	// push x to the left depending on the table size
	GUI::beginMain();

	// Colors
	int colorWhite[3] = {255, 255, 255};
	int colorBlack[3] = {0, 0, 0};
	int colorGray[3] = {100, 100, 100};
	int colorRed[3] = {255, 0, 0};
	for (int i = 0; i < rows; i++) {
		y += rectSize + spacing;

		if (GUI::isLowerThanScreen(y + rectSize)) continue;	 // If the rect is outside the screen, skip it
		if (GUI::isHigherThanScreen(y)) break;				 // If the rect is above the screen, break the loop
		x = START_X - spacing - rectSize;
		for (int j = 0; j < columns; j++) {
			x += rectSize + spacing;
			if (GUI::isLeftOfScreen(x + rectSize + spacing)) continue;	// If the rect is outside the screen, skip it
			if (GUI::isRightOfScreen(x)) break;							// If the rect is right of the screen, break the loop
			int index = i * columns + j;

			// Draw the node
			if (table->table->keys[index] == NULL) {
				// If the key is null, draw a black rect
				GUI::rect(x, y, rectSize, rectSize, colorBlack);
				// Index
				if (GUI::getScale() > 0.3)
					GUI::text(x, y, std::to_string(index), colorWhite);
			} else if (*table->table->keys[index] == "") {
				// The key is a tombstone so draw a gray rect with the RIP text
				GUI::rect(x, y, rectSize, rectSize, colorGray);
				if (GUI::getScale() > 0.3)
					GUI::text(x, y, "RIP", colorWhite);
			} else {
				// If the key exists, draw a white rect
				GUI::rect(x, y, rectSize, rectSize, (selectedKey == *table->table->keys[index]) ? colorRed : colorWhite);
				if (GUI::getScale() > 0.3) {
					// Draw the index, key and value
					GUI::text(x, y, std::to_string(index), colorBlack);
					GUI::text(x, y + 10, *table->table->keys[index], colorBlack);
					GUI::text(x, y + 20, std::to_string(table->table->values[index]), colorBlack);
				}
			}

			// Draw the line
			if (index != table->capacity - 1) {
				// Draw the line
				GUI::line(x + rectSize, y + rectSize / 2, x + rectSize + spacing, y + rectSize / 2, colorWhite);
				// Draw the arrow
				GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 - 10, colorWhite);
				GUI::line(x + rectSize + spacing, y + rectSize / 2, x + rectSize + spacing - 10, y + rectSize / 2 + 10, colorWhite);
			}
		}
	}
	IG::End();
}