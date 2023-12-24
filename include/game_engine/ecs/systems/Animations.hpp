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
        /**
         * @class AnimationSystem
         * @brief System class responsible for handling and updating animations within the ECS framework.
         *
         * Inherits from the base System class. This system manages the animation logic for entities
         * that have animation components.
         */
        class AnimationSystem : public System {
            public:
            /**
                 * @brief Handles and updates the animations for all relevant entities.
                 *
                 * This method iterates through entities with animation components and updates their
                 * animations based on the current game context.
                 */
                void handleAnimations();
        };
    }
}