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

            typedef struct animation_s {
                unsigned int animsCount;
                ModelAnimation *anims;
                int animFrameCounter;
                Model model;
            } animation_t;

        }
    }
}