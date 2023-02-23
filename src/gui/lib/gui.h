#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>
#include <sstream>
#include <imgui.h>
namespace GUI {
	namespace imgui = ImGui;
	int getHeight();
	int getWidth();
	// start main window and set size
	void beginMain();
	void circle(float x, float y, float radius, int[3]);
	void line(float x1, float y1, float x2, float y2, int[3]);
	void text(float x, float y, std::string text, int[3]);
	void rect(float x, float y, float width, float height, int[3]);

	int getMouseX();
	int getMouseY();

	bool isOnScreen(float x, float y);
	bool isRectOnScreen(float x, float y, float width, float height);

	bool windowShouldClose();
	int init();
	void render(void (*update)());
	int quit();
}  // namespace GUI

#endif