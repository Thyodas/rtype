/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.hpp
*/

#pragma once

#include <cstdint>

#include "ImGuizmo.h"

#include "game_engine/editor/ADocumentWindow.hpp"

#include "game_engine/GameEngine.hpp"

#include <raylib.h>

namespace engine::editor {
    class Main3DScene : public ADocumentWindow {
        public:
            Main3DScene();
            ~Main3DScene() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

        private:
            RenderTexture _viewTexture;
            ImVec2 _currentWindowSize = {0, 0};
            ImVec2 _prevWindowSize = {0, 0};
            ImVec2 _viewSize = {0, 0};
            ImVec2 _viewPosition = {0, 0};
            ecs::Entity _selectedEntity = 0;
            int _targetFPS = 60;
            ImGuizmo::OPERATION _currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE _currentGizmoMode = ImGuizmo::WORLD;
            ImGuizmo::OPERATION _lastGizmoOperationOver = ImGuizmo::SCALE;


            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //
            void setupWindow();
            void setupScene();
            void setupCamera();
            void setupTexture();
            void loadEntities();

            bool isWindowResized() const;
            void handleWindowResize();

            void renderToolbar();
            void renderGizmo();
            void renderView();
    };

} // namespace engine::editor
