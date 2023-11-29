/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.cpp
*/

#include "common/ecs/components/Physics.hpp"
#include "common/ecs/systems/Physics.hpp"
#include "common/ecs/Coordinator.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void PhysicsSystem::updatePosition(float dt) {
            for (auto const &entity : _entities) {
                auto& pos = gCoordinator.getComponent<components::physics::position_t>(entity);
                auto& vel = gCoordinator.getComponent<components::physics::velocity_t>(entity);

                pos.x += vel.x * dt;
                pos.y += vel.y * dt;
            }
        }
    }
}