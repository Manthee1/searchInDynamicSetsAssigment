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
static OAHashTable* table;
static std::string insertKey;
static int insertValue = 0;
static int highlightIndex = -1;
static std::string selectedKey = "";
static int selectedValue = 0;

static int rows = 0;
static int columns = 0;
static int size = 0;
static void updateTableDimensions() {
	rows = 1;
	columns = 1;
	while (rows * columns < table->capacity) (rows > columns) ? columns++ : rows++;
}

// timer
static int timer = 0;
void OAHashTableView::draw() {
	static char* insertKeyCharArray = new char[100]{"\0"};
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
		int newSelectedValue = table->searchKey(searchKey);
		if (newSelectedValue == -1) {
			selectedKey = "";
			timer = 200;
		} else {
			selectedKey = searchKey;
			selectedValue = newSelectedValue;
		}
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

	// Check if mouse clicked on a table entry
	if (IG::IsMouseClicked(0)) {
		// Get mouse position
		int mouseX = GUI::getMouseX();
		int mouseY = GUI::getMouseY();
		// Check if mouse is inside the table
		if (mouseX > START_X && mouseX < START_X + columns * (RECT_SIZE + SPACING) &&
			mouseY > START_Y && mouseY < START_Y + rows * (RECT_SIZE + SPACING)) {
			int clickNode = (mouseX - START_X) / (RECT_SIZE) + (mouseY - START_Y) / (RECT_SIZE)*columns;

			if (clickNode % 3 != 0 || table->table->keys[clickNode / 3] == NULL) return;
			clickNode /= 3;
			selectedKey = *(table->table->keys[clickNode]);
			selectedValue = table->table->values[clickNode];
		}
	}

	if (selectedKey != "") {
		IG::Begin("Selected Entry");
		IG::Text("Key: %s", selectedKey.c_str());
		IG::Text("Value: %d", selectedValue);
		if (IG::Button("Clear Selection")) {
			selectedKey = "";
			selectedValue = 0;
		}
		if (IG::Button("Clear Selection")) {
			selectedKey = "";
			selectedValue = 0;
		}
		if (IG::Button("Delete Entry")) {
			table->deleteKey(selectedKey);
			selectedKey = "";
			selectedValue = 0;
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

	int x = START_X - rectSize - spacing;
	int y = START_Y - rectSize - spacing;
	// push x to the left depending on the table size
	GUI::beginMain();

	// COlors
	int colorWhite[3] = {255, 255, 255};
	int colorBlack[3] = {0, 0, 0};
	int colorGray[3] = {100, 100, 100};
	int colorRed[3] = {255, 0, 0};
	for (int i = 0; i < rows; i++) {
		y += rectSize + spacing;
		if (GUI::isLowerThanScreen(y + rectSize))
			continue;
		if (GUI::isHigherThanScreen(y))
			break;
		x = START_X - spacing - rectSize;
		for (int j = 0; j < columns; j++) {
			x += rectSize + spacing;
			if (GUI::isLeftOfScreen(x + rectSize + spacing))
				continue;
			if (GUI::isRightOfScreen(x))
				break;
			int index = i * columns + j;

			// Draw the rect
			if (table->table->keys[index] == NULL) {
				GUI::rect(x, y, rectSize, rectSize, colorBlack);
				// Index
				GUI::text(x, y, std::to_string(index), colorWhite);
			} else if (*table->table->keys[index] == "") {
				// The key is a tombstone
				GUI::rect(x, y, rectSize, rectSize, colorGray);
				// Tombstone
				GUI::text(x, y, "RIP", colorWhite);
			} else {
				GUI::rect(x, y, rectSize, rectSize, (selectedKey == *table->table->keys[index]) ? colorRed : colorWhite);

				// Draw the index, key and value
				GUI::text(x, y, std::to_string(index), colorBlack);
				GUI::text(x, y + 10, *table->table->keys[index], colorBlack);
				GUI::text(x, y + 20, std::to_string(table->table->values[index]), colorBlack);
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