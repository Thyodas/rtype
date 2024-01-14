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
        : _camera(engine::createCamera(Vector3{7.0f, 7.0f, 7.0f}, Vector3{0.0f, 2.0f, 0.0f})),
        _sceneID(engine::createScene())
    {
        _opened = false;
        _modelLoaded = false;
    }

    ModelViewerWindow::~ModelViewerWindow()
    {
        if (_modelLoaded)
            engine::destroyEntity(_importedEntity);
    }

    void ModelViewerWindow::setup()
    {
        engine::attachCamera(_sceneID, _camera);
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

            if (!filePath) {
                _opened = false;
                return;
            }
            _assetPath = std::string(filePath);
            _importedEntity = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
            engine::addEntityToScene(_sceneID, _importedEntity);
            _modelLoaded = true;
        }
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
        ImGui::Begin("Import", &_opened, ImGuiWindowFlags_NoScrollbar);

        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        engine::renderTextureMode(_sceneID, _camera.getCameraID());
        rlImGuiImageRenderTexture(&_camera.getRenderTexture());

        ImGui::End();
   }


    void ModelViewerWindow::update()
    {
        if (!_opened)
            return;

        engine::update(_sceneID);
    }
}
