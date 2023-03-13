#include "CHashTableView.h"
#include <gui.h>
#include <imgui.h>
#include <cstdlib>
#define IG GUI::imgui

// CHashTable table = CHashTable(10, new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
static CHashTable* table;
static int insertValue = 0;
static int highlightIndex = -1;
static int highlightKeyIndex = -1;
// timer
static int timer = 0;
void CHashTableView::draw() {
	IG::Begin("Add Entry");
	static char* insertKey = new char[100]{"\0"};
	IG::InputText("Key", insertKey, 100);
	IG::InputInt("Value", &insertValue);
	if (IG::Button("Add Entry"))
		table->insertKey(insertKey, insertValue);
	IG::End();

	IG::Begin("Create Random Table");
	static int amount = 10;
	static int tableSize = 10;
	IG::InputInt("Amount", &amount);
	IG::InputInt("Size", &tableSize);
	if (IG::Button("Create Table")) {
		// Destroy the old table
		delete table;
		// Create a new table
		table = new CHashTable(tableSize);
		for (int i = 0; i < amount; i++) {
			std::string randomString = generateRandomString((int)(rand() % 12 + 3));
			table->insertKey(randomString, rand() % 100);
		}
	}
	IG::End();

	IG::Begin("Search Entry");
	static char* searchKey = new char[100]{"\0"};
	IG::InputText("Key", searchKey, 100);
	if (IG::Button("Search Entry")) {
		highlightIndex = table->hash(searchKey);
		highlightKeyIndex = table->getKeyIndex(searchKey, highlightIndex);
		if (highlightKeyIndex == -1) {
			highlightIndex = -1;
			timer = 200;
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
	static int deleteKey = 0;
	IG::InputInt("Key", &deleteKey);
	if (IG::Button("Delete Entry"))
		table->deleteKey(std::to_string(deleteKey));
	if (IG::Button("Clear Hash Table"))
		table->clear();
	IG::End();

	// If the table is not null, draw the table
	if (table == nullptr) return;

	drawTable();
	drawTableWindow();
}

static void drawTableWindow() {
	IG::Begin("Table");
	IG::Text("Key");
	IG::SameLine();
	IG::Text("Value");
	for (int i = 0; i < table->buckets; i++) {
		HashTableChain* entry = &table->table[i];
		for (int j = 0; j < entry->size; j++) {
			IG::Text("%s", (entry->keys[j]).c_str());
			IG::SameLine();
			IG::Text("%d", entry->values[j]);
		}
		// Add separator
		if (i != table->buckets - 1 && entry->size != 0) IG::Separator();
	}
	IG::End();
}

static void drawTable() {
	// Draw the table relations on the frame using rect
	int x = 300;
	GUI::beginMain();

	int colorWhite[3] = {255, 255, 255};
	int colorBlack[3] = {0, 0, 0};
	int colorRed[3] = {255, 0, 0};

	for (int i = 0; i < table->buckets; i++) {
		HashTableChain* entry = &table->table[i];

		float y = 50 * i + 10;

		// if the we can not see the table, do not draw it
		if (GUI::isLowerThanScreen(y + 60)) continue;
		if (GUI::isHigherThanScreen(y)) break;

		// Draw Rect
		GUI::rect(x, y, 25, 25, colorWhite);
		GUI::text(x + 10, y, std::to_string(i).c_str(), colorBlack);
		for (int j = 0; j < entry->size; j++) {
			// Draw the rect
			int entry_x = x + 50 * (j + 1);
			int entry_y = y;
			int selectColor[3] = {100, 255, 0};

			GUI::line(x + 12, 50 * i + 22, entry_x + 12, entry_y + 12, colorRed);
			GUI::rect(entry_x, entry_y, 25, 25, (highlightKeyIndex == j && highlightIndex == i) ? selectColor : colorWhite);
			GUI::text(entry_x + 2, entry_y, entry->keys[j], colorBlack);
			GUI::text(entry_x + 2, entry_y + 10, std::to_string(entry->values[j]).c_str(), colorBlack);
		}
		if (i != table->buckets - 1)
			GUI::line(x + 12, 50 * i + 22, x + 12, 50 * (i + 1) + 10, colorRed);
	}

	IG::End();
}