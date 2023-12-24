/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Animations.hpp
*/

#pragma once

#include "raylib.h"

namespace ecs {
    namespace components {
        namespace animations {

            /**
             * @struct animation_t
             * @brief Represents an animation component for use within the ECS framework.
             *
             * This struct is designed to encapsulate all necessary information for handling
             * animations in the ECS system..
             */
            typedef struct animation_s {
                unsigned int animsCount; ///< Number of animations in the 'anims' array.
                ModelAnimation *anims; ///< Pointer to an array of ModelAnimation objects.
                int animFrameCounter; ///< Counter for the current frame of the animation.
                Model model; ///< The model to which these animations apply.
            } animation_t;

        }
    }
}