/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/Main3DScene.hpp"

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/GameEngine.hpp"
#include "../../TestBehaviour.hpp"

class input;

engine::editor::Main3DScene::Main3DScene()
{
}

engine::editor::Main3DScene::~Main3DScene()
{
}

void engine::editor::Main3DScene::setup()
{
    ImVec2 pos = ImVec2(118, 24);
    ImVec2 size = ImVec2(1389, 804);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

    _viewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    //engine::initEngine();
    ecs::Entity cube = engine::createCube({0, 2, 0}, 4, 4, 4, RED, true);
    //engine::setRotation(cube, {deg2rad(30), 0, 0});
    //engine::setRotation(cube, {deg2rad(-10), 0, 0});
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);
    ecs::Entity cube2 = engine::createCube({5, 1, 0}, 2, 2, 2);
}

void engine::editor::Main3DScene::shutdown()
{
    UnloadRenderTexture(_viewTexture);
}

void engine::editor::Main3DScene::show()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

    if (ImGui::Begin("3D View", &_opened, ImGuiWindowFlags_NoScrollbar))
    {
        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        // draw the view
        rlImGuiImageRenderTextureFit(&_viewTexture, true);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::update()
{
    if (!_opened)
        return;
    if (IsWindowResized())
    {
        UnloadRenderTexture(_viewTexture);
        _viewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    engine::runEngineTextureMode(_viewTexture);
}
