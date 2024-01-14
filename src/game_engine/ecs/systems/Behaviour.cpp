/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.cpp
*/

#include "game_engine/ecs/systems/Behaviour.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/Coordinator.hpp"

namespace ecs {
    namespace system {
        void BehaviourSystem::handleBehaviours()
        {
            // create deep copy of _entities
            std::set<Entity> entities = _entities;

            for (auto const &entity : entities) {
                auto &behave = _coord->getComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity);

                behave->update();
            }
        }
    }
}