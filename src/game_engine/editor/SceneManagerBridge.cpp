/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneManagerBridge.cpp
*/

#include "game_engine/editor/SceneManagerBridge.hpp"

std::vector<ecs::SceneID> engine::editor::SceneManagerBridge::getSceneIDs() const
{
    return std::move(_sceneManager.getSceneIDs());
}

std::vector<ecs::Entity> engine::editor::SceneManagerBridge::getSceneEntities(ecs::SceneID sceneID) const
{
    return std::move(_sceneManager.getSceneEntities(sceneID));
}

std::vector<ecs::Entity> engine::editor::SceneManagerBridge::getAllEntities() const
{
    return std::move(_sceneManager.getAllEntities());
}
