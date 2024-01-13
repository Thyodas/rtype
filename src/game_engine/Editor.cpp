/*******************************************************************************************
*
*   raylib-extras [ImGui] example - editor
*
*	This is a more complex ImGui Integration
*	It shows how to build windows on top of 2d and 3d views using a render texture
*
*   Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/

#define IMGUI_ENABLE_FREETYPE

#include <imgui_impl_raylib.h>
#include "imgui.h"
#include "rlImGui.h"
//#include "rlImGuiColors.h"

#include <editor/DocumentWindows/EntityPropertiesWindow.hpp>
#include <editor/DocumentWindows/Main3DScene.hpp>
#include <editor/DocumentWindows/SceneTreeWindow.hpp>
#include <editor/DocumentWindows/ModelViewerWindow.hpp>

#include "game_engine/core/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"
#include "TestBehaviour.hpp"

#include "game_engine/editor/GameEngineEditor.hpp"

#include "raylib.h"
#include "raymath.h"

#include <iostream>

int main(int argc, char* argv[])
{
	engine::editor::GameEngineEditor editor;

	editor.registerWindow("3D View", std::make_shared<engine::editor::Main3DScene>());
	editor.registerWindow("Scene Tree", std::make_shared<engine::editor::SceneTreeWindow>());
	editor.registerWindow("Properties", std::make_shared<engine::editor::EntityPropertiesWindow>());
	editor.registerWindow("ModelViewer", std::make_shared<engine::editor::ModelViewerWindow>());
	editor.init();

	// Main game loop
	while (editor.isOpen())    // Detect window close button or ESC key
	{
		editor.update();
		editor.render();
	}

	return 0;
}
