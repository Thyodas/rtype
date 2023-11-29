/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Drawable.hpp
*/

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace ecs {
    namespace components {
        namespace drawable {
            typedef struct drawable_sfml_s {
                bool isRendered;
                sf::RenderWindow *window;
                sf::Sprite sprite;
            } drawable_sfml_t;
        }
    }
}