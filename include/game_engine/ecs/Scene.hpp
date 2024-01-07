/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Scene.hpp
*/

#pragma once

#include "Entity.hpp"

#include <cstddef>
#include <set>
#include <unordered_map>
#include <iostream>

namespace ecs {
    using SceneID = std::size_t;

    class Scene {
        public:
            SceneID _id;
            std::set<ecs::Entity> entities;

            Scene(SceneID id) : _id(id) {};

            void addEntity(ecs::Entity entity)
            {
                entities.insert(entity);
            }
            void removeEntity(ecs::Entity entity)
            {
                entities.erase(entity);
            }
    };

    class SceneManager {
        public:
            void updateActiveEntities(void)
            {
                activeEntities.clear();
                for (const auto& sceneId : activeScenes) {
                    const auto &scene = scenes.at(sceneId);
                    activeEntities.insert(scene.entities.begin(), scene.entities.end());
                }
            }

            void addEntityToScene(ecs::Entity entity, SceneID sceneID)
            {
                scenes.at(sceneID).addEntity(entity);
                updateActiveEntities();
            }

            void removeEntityFromScene(ecs::Entity entity, SceneID sceneID)
            {
                scenes.at(sceneID).removeEntity(entity);
                updateActiveEntities();
            }

            void entityDestroyed(ecs::Entity entity)
            {
                for (auto &[id, scene] : scenes)
                    scene.removeEntity(entity);
                updateActiveEntities();
            }

            void createScene(SceneID id)
            {
                scenes.emplace(id, Scene{id});
            }

            void deleteScene(SceneID id)
            {
                scenes.erase(id);
            }

            void activateScene(SceneID sceneID)
            {
                activeScenes.insert(sceneID);
                inactiveScenes.erase(sceneID);
                updateActiveEntities();
            }

            void deactivateScene(SceneID sceneID)
            {
                activeScenes.erase(sceneID);
                inactiveScenes.insert(sceneID);
                updateActiveEntities();
            }
            const std::set<ecs::Entity> &getActiveEntities(void) const
            {
                return activeEntities;
            }

        private:
            std::unordered_map<SceneID, Scene> scenes;
            std::set<SceneID> activeScenes;
            std::set<SceneID> inactiveScenes;
            std::set<ecs::Entity> activeEntities;
    };
}