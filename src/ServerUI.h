﻿#pragma once
#include <vector>

#include <iostream>
#include <ctime>

#include "server/interface/container.h"
#include "server/interface/router.h"

#include "imgui.h"
#include "../bindings/imgui_impl_glfw.h"
#include "../bindings/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
template <typename Derived>
class UI
{
public:
	UI()
	{
		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			std::exit(1);

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
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
		window = glfwCreateWindow(720, 480, "SERVER", NULL, NULL);
		if (window == NULL)
			std::exit(1);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
	}
	~UI()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Run()
	{
		StartUp();

		while (!glfwWindowShouldClose(window))
		{
			// Poll and handle events (inputs, window resize, etc.)
	  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	  // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			Update();

			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
	}
	void Update()
	{
		static_cast<Derived*>(this)->Update();
	}
	void StartUp()
	{
		static_cast<Derived*>(this)->StartUp();
	}

protected:
	GLFWwindow* window;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

class myUI : public UI<myUI>
{
public:

	myUI() { }
	~myUI() = default;

	void StartUp()
	{
	}

	void DrawRowsBackground(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float y0 = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

		int row_display_start;
		int row_display_end;
		ImGui::CalcListClipping(row_count, line_height, &row_display_start, &row_display_end);
		for (int row_n = row_display_start; row_n < row_display_end; row_n++)
		{
			ImU32 col = (row_n & 1) ? col_odd : col_even;
			if ((col & IM_COL32_A_MASK) == 0)
				continue;
			float y1 = y0 + (line_height * row_n);
			float y2 = y1 + line_height;
			draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
		}
	}

	void Update()
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(224, 255, 255)));
		ImGui::Begin("Settings", 0);
		ImGui::PopStyleColor();

		if (isServerRunning) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 255, 0)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(0, 255, 0)));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(0, 255, 0)));	
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0)));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(255, 0, 0)));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(255, 0, 0)));
		}

		if (ImGui::Button("Run Server")) {
			isServerRunning = !isServerRunning;
			now = time(0);
			dt = ctime(&now);
			if (isServerRunning) logs.push_back(dt + "The server has started.\n");
			else logs.push_back(dt + "The server has stopped.\n");
		}
		
		ImGui::PopStyleColor(4);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0)));
		ImGui::Text("Logs information:");
		ImGui::PopStyleColor();

		if (ImGui::BeginListBox("Logs information:", ImVec2(200, 150))) {
			for (int i = 0; i < logs.size(); ++i) {
				const bool isSelected = (selectedIndex == i);
				
				if (ImGui::Selectable(logs[i].c_str(), isSelected)) {
					selectedIndex = i;
				}
				/*float x1 = ImGui::GetWindowContentRegionMin().x;
				float x2 = ImGui::GetWindowContentRegionMax().x;
				float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
				float item_offset_y = -item_spacing_y * 0.5f;
				float line_height = ImGui::GetTextLineHeight() + item_spacing_y;
				DrawRowsBackground(logs.size() - 1, line_height, x1, x2, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, 0.5f)));*/
				// Set the initial focus when opening the combo
				// (scrolling + keyboard navigation focus)
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

	}


public:
	bool show_demo_window = true;
	bool show_another_window = false;
	bool isServerRunning = false;
	
	std::vector<std::string> logs;
	int selectedIndex = 0;

	time_t now;
	std::string dt;
};