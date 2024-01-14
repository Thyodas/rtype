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
    enum SceneObjectType {
        SCENE_OBJECT_TYPE_ENTITY,
        SCENE_OBJECT_TYPE_SCENE,
        SCENE_OBJECT_TYPE_FOLDER,
        SCENE_OBJECT_TYPE_UNKNOWN
    };

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
            uint32_t id;
            SceneObjectType type = SCENE_OBJECT_TYPE_UNKNOWN;
        };

        SceneObject root_;
        void ShowNode(SceneObject& object);
    };
}
