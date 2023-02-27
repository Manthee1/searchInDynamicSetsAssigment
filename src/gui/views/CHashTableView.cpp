#include "CHashTableView.h"
#include <gui.h>
#include <imgui.h>
#include <cstdlib>
#define IG GUI::imgui

// CHashTable table = CHashTable(10, new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
static CHashTable* table;
static int insertKey = 0;
static int insertValue = 0;
static int highlightIndex = -1;
static int highlightKeyIndex = -1;
// timer
static int timer = 0;
void CHashTableView::draw() {
	IG::Begin("Add Entry");
	IG::InputInt("Key", &insertKey);
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
		table->~CHashTable();
		// Create a new table
		table = new CHashTable(tableSize);
		for (int i = 0; i < amount; i++)
			table->insertKey(rand() % amount, rand() % amount);
	}
	IG::End();

	IG::Begin("Search Entry");
	static int searchKey = 0;
	IG::InputInt("Key", &searchKey);
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
		table->deleteKey(deleteKey);
	IG::End();

	drawTable();
	drawTableWindow();
}

static void drawTableWindow() {
	IG::Begin("Table");
	IG::Text("Key");
	IG::SameLine();
	IG::Text("Value");
	for (int i = 0; i < table->capacity; i++) {
		HashTableChain* entry = &table->table[i];
		for (int j = 0; j < entry->size; j++) {
			IG::Text("%d", entry->keys[j]);
			IG::SameLine();
			IG::Text("%d", entry->values[j]);
		}
		// Add separator
		if (i != table->capacity - 1 && entry->size != 0) IG::Separator();
	}
	IG::End();
}

static void drawTable() {
	// Draw the table relations on the frame using rect
	int x = 300;
	GUI::beginMain();
	for (int i = 0; i < table->capacity; i++) {
		HashTableChain* entry = &table->table[i];

		// Draw Rect
		GUI::rect(x, 50 * i + 10, 25, 25, new int[3]{255, 255, 255});
		GUI::text(x + 10, 50 * i + 10, std::to_string(i).c_str(), new int[3]{0, 0, 0});
		for (int j = 0; j < entry->size; j++) {
			// Draw the rect
			int entry_x = x + 50 * (j + 1);
			int entry_y = 50 * i + 10;
			int color[3];
			if (highlightKeyIndex == j && highlightIndex == i) {
				color[0] = 100;
				color[1] = 255;
				color[2] = 0;
			} else {
				color[0] = 255;
				color[1] = 255;
				color[2] = 255;
			}
			GUI::line(x + 12, 50 * i + 22, entry_x + 12, entry_y + 12, new int[3]{255, 0, 0});
			GUI::rect(entry_x, entry_y, 25, 25, color);
			GUI::text(entry_x + 10, entry_y, std::to_string(entry->keys[j]).c_str(), new int[3]{0, 0, 0});
			GUI::text(entry_x + 10, entry_y + 10, std::to_string(entry->values[j]).c_str(), new int[3]{0, 0, 0});
		}
		if (i != table->capacity - 1)
			GUI::line(x + 12, 50 * i + 22, x + 12, 50 * (i + 1) + 10, new int[3]{255, 0, 0});
	}
	IG::End();
}