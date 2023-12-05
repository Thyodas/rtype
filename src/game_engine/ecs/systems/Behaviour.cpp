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
        void BehaviourSystem::handleBehaviours(ecs::Coordinator &coord) {
            for (auto const &entity : _entities) {
                auto &transf = coord.getComponent<ecs::components::physics::transform_t>(entity);
                auto &behave = coord.getComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity);

                behave->update(transf);
            }
        }
    }
}