/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Lights.hpp
*/

#pragma once

#include "my_raylib.h"

namespace ecs::components::lights {

    struct LightSource {
        Vector2 position;
        Color color;
        float intensity;

        LightSource(Vector2 pos = {0, 0}, Color col = WHITE, float inten = 1.0f)
            : position(pos), color(col), intensity(inten) {}
    };

}
