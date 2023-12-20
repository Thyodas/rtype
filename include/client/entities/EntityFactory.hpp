/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#pragma once

#include "game_engine/GameEngine.hpp"
#include "Objects.hpp"

#include <memory>

namespace client {
    class EntityFactory {
        public:
            ecs::Entity createEntity(client::ObjectType type, client::ObjectName name, client::ObjectParams params);
            ecs::Entity createCube(client::ObjectName name, client::ObjectParams params);
            ecs::Entity createObject3D(client::ObjectName name, client::ObjectParams params);
            std::string objectNameToString(client::ObjectName name);
    };
}
