/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneTreeWindow.hpp
*/

#pragma once

#include <vector>
#include <string>
#include "editor/ADocumentWindow.hpp"

namespace engine::editor {
    class SceneTreeWindow : public ADocumentWindow {
        public:
        SceneTreeWindow();
        ~SceneTreeWindow() override;

        void setup() override;
        void shutdown() override;
        void show() override;
        void update() override;

        private:
        struct SceneObject {
            std::string name;
            std::vector<SceneObject> children;
        };

        SceneObject root_;
        void ShowNode(SceneObject& object);
    };
}
