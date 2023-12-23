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

namespace common::game {
    class EntityFactory {
        public:
            ecs::Entity createEntity(
                ObjectType type,
                ObjectName name,
                ObjectParams params,
                ObjectFormat format = ObjectFormat::OBJ
            );
        private:
            ecs::Entity createCube(
                ObjectName name,
                ObjectParams params,
                ObjectFormat format = ObjectFormat::OBJ
            );
            ecs::Entity createObject3D(
                ObjectName name,
                ObjectParams params,
                ObjectFormat format = ObjectFormat::OBJ
            );
            std::string objectNameToString(ObjectName name);
            std::string objectFormatToString(ObjectFormat format);
    };
}
