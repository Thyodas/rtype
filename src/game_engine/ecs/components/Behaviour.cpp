/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.cpp
*/

#include "game_engine/ecs/components/Behaviour.hpp"

namespace ecs {
    namespace components {
        namespace behaviour {

            void Behaviour::setEntity(ecs::Entity entity)
            {
                _entity = entity;
            }

        }
    }
}