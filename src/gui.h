#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
namespace GUI {
	namespace imgui = ImGui;
	void circle(float x, float y, float radius, int[3]);
	void line(float x1, float y1, float x2, float y2, int[3]);
	bool windowShouldClose();
	int init();
	void render(void (*update)());
	int quit();
}  // namespace GUI
