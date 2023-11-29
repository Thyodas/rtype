/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Controls.hpp
*/

#pragma once

namespace ecs {
    namespace components {
        namespace controls {
            typedef struct controllable_s {
                bool isMovingLeft;
                bool isMovingUp;
                bool isMovingRight;
                bool isMovingDown;
            } controllable_t;
        }
    }
}