#ifndef IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "imgui.h"
#include "backend/imgui_impl_glfw.h"
#include "./backend/imgui_impl_opengl3.h"
#include <stdio.h>
#include <sstream>
#include <math.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "./backend/glfw3.h"

#ifndef GLFW_TRANSPARENT_FRAMEBUFFER
#define GLFW_TRANSPARENT_FRAMEBUFFER 0x00033002
#endif 

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

ImGuiIO io;
ImVec4 clear_color = ImVec4(0.15, 0.15, 0.15, 1.00f);

#include "gui.h"

namespace GUI {
	GLFWwindow* window;
	namespace imgui = ImGui;
	int window_width = 1280;
	int window_height = 720;
	float scale = 1.0f;
	float offset_x = 0;
	float offset_y = 0;
	bool mouse_down = false;

	bool windowShouldClose() {
		return glfwWindowShouldClose(window);
	}
	int init() {
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			return 1;

		// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
		const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
		// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on Mac
#else
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		// Create window with graphics context
		window = glfwCreateWindow(window_width, window_height, "Search in DSA", NULL, NULL);
		if (window == NULL)
			return 1;
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);  // Enable vsync

		// Add close button
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			glfwSetWindowShouldClose(window, true);
			});

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	 // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	 // Enable Multi-Viewport / Platform Windows
		// io.ConfigViewportsNoAutoMerge = true;
		// io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Update screen size
		window_width = ImGui::GetIO().DisplaySize.x;
		window_height = ImGui::GetIO().DisplaySize.y;

		return 0;
	}

	void render(void (*update)()) {
		glfwPollEvents();

		// Start the Dear ImGui invisible frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Update screen size
		window_width = ImGui::GetIO().DisplaySize.x;
		window_height = ImGui::GetIO().DisplaySize.y;

		// Update
		update();

		// Listen for scroll
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
			if (ImGui::GetIO().MouseWheel != 0) {
				float old_scale = scale;
				// Make the scaling factor slower then smaller it is
				scale += ImGui::GetIO().MouseWheel * 0.1f * scale;
				if (scale < 0.05f) scale = 0.05f;
				if (scale > 10.0f) scale = 10.0f;

				// // Change the offset so that it appears that the mouse is still hovering over the same point
				// offset_x += (ImGui::GetIO().MousePos.x - offset_x) * (old_scale - scale) / old_scale;
				// offset_y += (ImGui::GetIO().MousePos.y - offset_y) * (old_scale - scale) / old_scale;
				// Make sure to zoom towards the center of the screen
				offset_x -= (window_width / old_scale - window_width / scale) / 2;
				offset_y -= (window_height / old_scale - window_height / scale) / 2;
			}
		}
		mouse_down = ImGui::IsMouseDown(0);
		if (mouse_down) {
			offset_x += ImGui::GetIO().MouseDelta.x / scale;
			offset_y += ImGui::GetIO().MouseDelta.y / scale;
		}
		// Scale and offset
		ImGui::Begin("Info");
		ImGui::Text("Scale: %f", scale);
		ImGui::Text("Offset: (%f, %f)", offset_x, offset_y);
		ImGui::Text("Size: (%f, %f)", window_width / scale, window_height / scale);
		if (ImGui::Button("Reset")) {
			scale = 1.0f;
			offset_x = 0;
			offset_y = 0;
		}
		ImGui::End();

		// Draw a crosshair
		beginMain();
		ImGui::GetWindowDrawList()->AddLine(ImVec2(window_width / 2 - 10, window_height / 2), ImVec2(window_width / 2 + 10, window_height / 2), IM_COL32(255, 255, 255, 255));
		ImGui::GetWindowDrawList()->AddLine(ImVec2(window_width / 2, window_height / 2 - 10), ImVec2(window_width / 2, window_height / 2 + 10), IM_COL32(255, 255, 255, 255));
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;

		// glfwGetFramebufferSize(window, &width, & window_height);
		// glViewport(0, 0, width, window_height);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}
	int quit() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();

		return 0;
	}

	ImVec2 getVec2(float x, float y) {
		return ImVec2((x + offset_x) * scale, (y + offset_y) * scale);
	}

	int getHeight() {
		return window_height;
	}
	int getWidth() {
		return window_width;
	}

	float getOffsetX() {
		return offset_x;
	}
	float getOffsetY() {
		return offset_y;
	}

	float getScale() {
		return scale;
	}

	int getMouseX() {
		return (ImGui::GetIO().MousePos.x / scale) - offset_x;
	}
	int getMouseY() {
		return (ImGui::GetIO().MousePos.y / scale) - offset_y;
	}

	void beginMain() {
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
		ImGui::Begin("Main Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
	}

	bool isOnScreen(float x, float y) {
		return x > -offset_x && x < -offset_x + window_width / scale && y > -offset_y && y < -offset_y + window_height / scale;
	}

	bool isRectOnScreen(float x, float y, float width, float height) {
		return x + width > -offset_x && x < -offset_x + window_width / scale && y + height > -offset_y && y < -offset_y + window_height / scale;
	}

	void focusOn(float x, float y, float newScale) {
		scale = newScale;
		offset_x = -x + window_width / (2 * scale);
		offset_y = -y + window_height / (2 * scale);
	}

	bool isLeftOfScreen(float x) {
		return x < -offset_x;
	}
	bool isRightOfScreen(float x) {
		return x > -offset_x + window_width / scale;
	}
	bool isLowerThanScreen(float y) {
		return y < -offset_y;
	}
	bool isHigherThanScreen(float y) {
		return y > -offset_y + window_height / scale;
	}

	void circle(float x, float y, float radius, int* color) {
		// Draw a circle on main window
		ImGui::GetWindowDrawList()->AddCircleFilled(getVec2(x, y), radius * scale, ImGui::GetColorU32(ImVec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f)));
	}

	void line(float x1, float y1, float x2, float y2, int* color) {
		// Draw a line on main window
		ImGui::GetWindowDrawList()->AddLine(getVec2(x1, y1), getVec2(x2, y2), ImGui::GetColorU32(ImVec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f)));
	}

	void text(float x, float y, std::string text, int* color) {
		// Draw a text on main window
		ImGui::GetWindowDrawList()->AddText(getVec2(x, y), ImGui::GetColorU32(ImVec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f)), text.c_str());
	}

	void rect(float x, float y, float width, float height, int* color) {
		// Draw a rectangle on main window
		ImGui::GetWindowDrawList()->AddRectFilled(getVec2(x, y), getVec2(x + width, y + height), ImGui::GetColorU32(ImVec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f)));
	}
};

#endif
