/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.hpp
*/

#pragma once

#include "../System.hpp"
#include "../Coordinator.hpp"

namespace ecs {
    namespace system {
        class BehaviourSystem : public System {
            public:
                void handleBehaviours(ecs::Coordinator &coord);
        };
    }
}