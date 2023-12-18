/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Animations.hpp
*/

#pragma once

#include "../System.hpp"
#include "../Coordinator.hpp"

namespace ecs {
    namespace system {
        class AnimationSystem : public System {
            public:
                void handleAnimations();
        };
    }
}