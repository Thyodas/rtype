/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

namespace ecs {
    namespace components {
        namespace physics {
            typedef struct position_s {
                float x;
                float y;
            } position_t;

            typedef struct velocity_s {
                float x;
                float y;
            } velocity_t;

            typedef struct acceleration_s {
                float x;
                float y;
            } acceleration_t;
        }
    }
}