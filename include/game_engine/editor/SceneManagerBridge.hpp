/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneManagerBridge.hpp
*/

#pragma once
#include <ecs/Scene.hpp>

namespace engine::editor {

    class SceneManagerBridge {
        public:
            SceneManagerBridge(const SceneManagerBridge&) = delete;
            SceneManagerBridge& operator=(const SceneManagerBridge&) = delete;

            // Singleton
            static SceneManagerBridge& getInstance(ecs::SceneManager& sceneManager)
            {
                static SceneManagerBridge instance(sceneManager);
                return instance;
            }

            [[nodiscard]] std::vector<ecs::SceneID> getSceneIDs() const;
            [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(ecs::SceneID sceneID) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;

        private:
            explicit SceneManagerBridge(ecs::SceneManager& sceneManager) : _sceneManager(sceneManager) {}
            ~SceneManagerBridge() = default;

            ecs::SceneManager& _sceneManager;
    };

}
