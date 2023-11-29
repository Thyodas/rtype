/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"

namespace ecs {
    namespace system {
        class PhysicsSystem : public System {
            public:
                void updatePosition(float dt);
        };
    }
}