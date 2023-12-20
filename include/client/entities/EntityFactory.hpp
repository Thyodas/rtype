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
            ecs::Entity createEntity(
                client::ObjectType type,
                client::ObjectName name,
                client::ObjectParams params,
                client::ObjectFormat format = client::ObjectFormat::OBJ
            );
        private:
            ecs::Entity createCube(
                client::ObjectName name,
                client::ObjectParams params,
                client::ObjectFormat format = client::ObjectFormat::OBJ
            );
            ecs::Entity createObject3D(
                client::ObjectName name,
                client::ObjectParams params,
                client::ObjectFormat format = client::ObjectFormat::OBJ
            );
            std::string objectNameToString(client::ObjectName name);
            std::string objectFormatToString(client::ObjectFormat format);
    };
}
