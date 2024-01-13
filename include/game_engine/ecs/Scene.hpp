/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Scene.hpp
*/

#pragma once

#include "Entity.hpp"
#include "core/Camera.hpp"

#include <cstddef>
#include <set>
#include <unordered_map>
#include <map>
#include <iostream>

namespace ecs {
    using SceneID = std::size_t;

    class Scene {
        public:
            SceneID _id;
            std::set<ecs::Entity> entities;
            bool isPaused = false;

            Scene(SceneID id) : _id(id) {};

            void addEntity(ecs::Entity entity)
            {
                entities.insert(entity);
            }

            void removeEntity(ecs::Entity entity)
            {
                entities.erase(entity);
            }

            void attachCamera(engine::core::EngineCamera &camera)
            {
                _cameras.emplace(camera._id, camera);
            }

            void detachCamera(engine::core::EngineCamera &camera)
            {
                _cameras.erase(camera._id);
            }

            engine::core::EngineCamera &getCamera(engine::core::CameraID id)
            {
                return _cameras.at(id);
            }

        private:
            std::unordered_map<engine::core::CameraID, engine::core::EngineCamera> _cameras;
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
                updateActiveEntities();
            }

            void deactivateScene(SceneID sceneID)
            {
                activeScenes.erase(sceneID);
                updateActiveEntities();
            }

            bool isScenePaused(SceneID sceneID)
            {
                return scenes.at(sceneID).isPaused;
            }

            bool isSceneActive(SceneID sceneID)
            {
                return activeScenes.find(sceneID) != activeScenes.end();
            }

            void pauseScene(SceneID sceneID)
            {
                scenes.at(sceneID).isPaused = true;
            }

            void resumeScene(SceneID sceneID)
            {
                scenes.at(sceneID).isPaused = false;
            }

            const std::set<ecs::Entity> &getActiveEntities(void) const
            {
                return activeEntities;
            }

            void attachCamera(SceneID id, engine::core::EngineCamera &camera)
            {
                scenes.at(id).attachCamera(camera);
            }

            void detachCamera(SceneID id, engine::core::EngineCamera &camera)
            {
                scenes.at(id).detachCamera(camera);
            }

            engine::core::EngineCamera &getCamera(SceneID id, engine::core::CameraID cameraID)
            {
                return scenes.at(id).getCamera(cameraID);
            }

        private:
            std::unordered_map<SceneID, Scene> scenes;
            std::set<SceneID> activeScenes;
            std::set<ecs::Entity> activeEntities;
    };
}