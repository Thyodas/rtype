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
    }
}