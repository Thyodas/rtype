/*
** EPITECH PROJECT, 2024
** server
** File description:
** GameEngineEditor.cpp
*/

#define IMGUI_ENABLE_FREETYPE

#include <imgui_impl_raylib.h>
#include "imgui.h"
#include "rlImGui.h"
//#include "rlImGuiColors.h"

#include "game_engine/editor/GameEngineEditor.hpp"

#include <ranges>
#include <utility>

#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"

#include "raylib.h"


engine::editor::GameEngineEditor::GameEngineEditor()
{
	setupEngine();
	setupStyle();
	setupDockspace();
}

bool engine::editor::GameEngineEditor::isOpen() const
{
	return !_quit && engine::isWindowOpen();
}

void engine::editor::GameEngineEditor::setupEngine()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
	engine::initEngine(false, 1920, 1000);

	rlImGuiBeginInitImGui();
	ImGui::StyleColorsDark();

	setupFonts();
	rlImGuiEndInitImGui();
}

void engine::editor::GameEngineEditor::setupStyle()
{
	ImGui::Spectrum::StyleColorsSpectrum();



	// Retrieve DPI scale
	Vector2 scale = GetWindowScaleDPI();
	if (scale.x > 1.0f || scale.y > 1.0f)
		std::cerr << "WARNING: High DPI detected! If you have rendering issues please try another scaling factor." << std::endl;

	ImGuiIO& io = ImGui::GetIO();
	io.DisplayFramebufferScale = ImVec2(scale.x, scale.y); // Apply the DPI scale to ImGui rendering
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle *style = &ImGui::GetStyle();
	style->CircleTessellationMaxError = 0.10f;
	style->WindowRounding = 10.0f;
	style->ChildRounding = 6.0f;
	style->PopupRounding = 4.0f;
	//style->ScaleAllSizes(scale.x);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Tab]                    = ImVec4(0.26f, 0.52f, 0.83f, 0.93f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.12f, 0.52f, 0.99f, 0.80f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.06f, 0.32f, 0.63f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.15f, 0.44f, 0.79f, 1.00f);
}

void engine::editor::GameEngineEditor::setupFonts()
{
	ImFontConfig font_config;
	font_config.OversampleH = 3; // Horizontal oversampling
	font_config.OversampleV = 3; // Vertical oversampling

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();

	float fontSize = 18.0f;

	ImFont* font = io.Fonts->AddFontFromFileTTF("resources/SourceSans3-Regular.ttf", fontSize, &font_config);
	IM_ASSERT(font != nullptr);
	io.FontDefault = font;

	/*
	ImFontConfig fontawesome_config;
	fontawesome_config.MergeMode = true;
	fontawesome_config.OversampleH = 3; // Horizontal oversampling
	fontawesome_config.OversampleV = 3; // Vertical oversampling
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("resources/fontawesome4.ttf", fontSize, &fontawesome_config, icon_ranges);
	*/

	Imgui_ImplRaylib_BuildFontAtlas();
}

void engine::editor::GameEngineEditor::setupDockspace()
{
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

}

void engine::editor::GameEngineEditor::drawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				_quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			/*
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
			//ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
			ImGui::MenuItem("3D View", nullptr, &SceneView.Open);
			*/

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


engine::editor::GameEngineEditor::~GameEngineEditor()
{
	rlImGuiShutdown();
	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();       // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
}

void engine::editor::GameEngineEditor::init()
{
	for (const auto& window: _windows | std::views::values) {
		window->setup();
	}
}

void engine::editor::GameEngineEditor::update()
{
	//engine::runEngineTextureMode();
	for (const auto& window: _windows | std::views::values) {
		if (window->isFocused())
			window->update();
	}
}

void engine::editor::GameEngineEditor::render()
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

	rlImGuiBegin();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	drawMenuBar();
	ImGui::ShowDemoWindow();

	for (const auto& window: _windows | std::views::values) {
		if (window->isOpened())
			window->show();
	}

	rlImGuiEnd();

	EndDrawing();
}

void engine::editor::GameEngineEditor::destroy()
{
	for (const auto& window: _windows | std::views::values) {
		window->shutdown();
	}
}

void engine::editor::GameEngineEditor::registerWindow(const std::string& name, std::shared_ptr<IDocumentWindow> window)
{
	_windows[name] = std::move(window);
}
