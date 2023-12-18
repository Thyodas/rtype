/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.hpp
*/

#pragma once

#include "raylib.h"
#include "Shapes.hpp"

#include <memory>

namespace ecs {
    namespace components {
        namespace render {
            typedef struct render_s {
                ShapeType type;
                bool isRendered;
                std::shared_ptr<components::IShape> data;
            } render_t;

        }
    }
}