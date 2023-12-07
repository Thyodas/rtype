/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"
#include "../components/Physics.hpp"
#include "../../../../src/game_engine/core/event/CollisionEvent.hpp"

namespace ecs {
    namespace system {
        class PhysicsSystem : public System {
            public:
                void updatePosition(ecs::Coordinator &coord);
        };
        class ColisionDetectionSystem : public System {
            public:
                void detectCollision(ecs::Coordinator &coord);
            private:
                void detectSphereCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
                void detectSphereBoxCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
        };

        class CollisionResponse {
            public:
                CollisionResponse(ecs::Coordinator &coord) : _coord(coord) {
                    std::cout << "ici" << std::endl;
                    _coord.registerListener<CollisionEvent>([](const CollisionEvent &event) {
                        std::cout << "collision between " << event.entity1 << " and " << event.entity2 << std::endl;
                    });
                };
            private:
                ecs::Coordinator& _coord;
        };
    }
}