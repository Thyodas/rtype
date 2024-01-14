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

#include "imgui.h"


#include "rlImGui.h"
//#include "rlImGuiColors.h"
#include <imgui_impl_raylib.h>

#include <editor/DocumentWindows/ConsoleWindow.hpp>
#include <editor/DocumentWindows/EntityPropertiesWindow.hpp>
#include <editor/DocumentWindows/Main3DScene.hpp>
#include <editor/DocumentWindows/SceneTreeWindow.hpp>
#include <editor/DocumentWindows/ModelViewerWindow.hpp>
#include <loguru/loguru.hpp>

#include "game_engine/core/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"

#include "game_engine/editor/GameEngineEditor.hpp"
#include "game_engine/editor/SceneManagerBridge.hpp"

#include "raylib.h"
#include "raymath.h"

#include <iostream>

#include "common/utils/Math.hpp"

int main(int argc, char* argv[])
{
	loguru::init(argc, argv);
	engine::editor::GameEngineEditor editor;

	editor.registerWindow("3D View", std::make_shared<engine::editor::Main3DScene>());
	editor.registerWindow("Scene Tree", std::make_shared<engine::editor::SceneTreeWindow>());
	editor.registerWindow("Properties", std::make_shared<engine::editor::EntityPropertiesWindow>());
	editor.registerWindow("ModelViewer", std::make_shared<engine::editor::ModelViewerWindow>());
	editor.registerWindow("Console", std::make_shared<engine::editor::ConsoleWindow>(editor));
	editor.init();

	// Main game loop
	while (editor.isOpen())    // Detect window close button or ESC key
	{
		editor.update();
		editor.render();
	}
	// engine::initEngine();
	// ecs::SceneID scene = engine::createScene();
	// engine::core::EngineCamera camera = engine::createCamera({-25, 8, 0}, {0, 0, 0}, {0, 1, 0}, CAMERA_PERSPECTIVE);
	// engine::attachCamera(scene, camera);
	// ecs::Entity cube = engine::createCube({-5, 20, -5}, 4, 4, 4, RED);
	// ecs::Entity model3D = engine::createModel3D("./ressources/client/Objects3D/DualStriker/DualStriker.obj", {0, 0, 0});
	// ecs::Entity floor = engine::createCube({0, -1, 0}, 100, 1, 100, WHITE, ecs::components::physics::BodyType::STATIC);
	// engine::addEntityToScene(cube, scene);
	// engine::addEntityToScene(floor, scene);
	// engine::addEntityToScene(model3D, scene);
	// engine::activateScene(scene);
	// engine::setRotation(cube, {40, 0, 0});
	// //engine::scale(cube, {1, 1, 2});
	// while (engine::isWindowOpen()) {
	// 	engine::update(scene);
	// 	engine::render(scene, camera._id);
	// 	JPH::Vec3 movementDirection(0, 0, 0);
	// 	if (IsKeyDown(KEY_D)) movementDirection += JPH::Vec3(0, 0, 1);
    //     if (IsKeyDown(KEY_A))  movementDirection += JPH::Vec3(0, 0, -1);
    //     if (IsKeyDown(KEY_W))    movementDirection += JPH::Vec3(1, 0, 0);
    //     if (IsKeyDown(KEY_S))  movementDirection += JPH::Vec3(-1, 0, 0);
	// 	float speed = 1.0f;
	// 	if (!movementDirection.IsNearZero())
    //         movementDirection = movementDirection.Normalized() * speed;
	// 	engine::physics::setLinearVelocity(cube, common::utils::joltVectorToRayVector(movementDirection));
	// }
	return 0;
}
