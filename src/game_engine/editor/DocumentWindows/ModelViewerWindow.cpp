/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ModelViewerWindow.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/ModelViewerWindow.hpp"

#include <iostream>
#include "tinyfiledialogs.h"

#include <string>

#include "game_engine/GameEngine.hpp"

namespace engine::editor {
    ModelViewerWindow::ModelViewerWindow()
    {
        _opened = false;
        _modelLoaded = false;

        engine::camera::setPosition(Vector3{7.0f, 7.0f, 7.0f});
        engine::camera::setTarget(Vector3{0.0f, 2.0f, 0.0f});
        _viewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    ModelViewerWindow::~ModelViewerWindow()
    {
    }

    void ModelViewerWindow::setup()
    {
    }

    void ModelViewerWindow::shutdown()
    {
    }

   void ModelViewerWindow::show() {
        if (!_opened) return;

        if (!_modelLoaded) {
            const char* filePath = tinyfd_openFileDialog(
                "Open File",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (filePath) {
                _assetPath = std::string(filePath);
                ecs::Entity cube = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
                _modelLoaded = true;
            }
        }
        ImGui::Begin("Import", &_opened, ImGuiWindowFlags_NoScrollbar);

        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        rlImGuiImageRenderTexture(&_viewTexture);

        ImGui::End();
   }


    void ModelViewerWindow::update()
    {
        if (!_opened)
            return;

        engine::runEngineTextureMode(_viewTexture);
    }
}
