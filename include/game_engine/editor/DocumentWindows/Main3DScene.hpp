/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.hpp
*/

#pragma once

#include "game_engine/editor/ADocumentWindow.hpp"

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
    };
} // namespace engine::editor
