/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Event.hpp
*/

#include "common/math/vectors/Vec2.hpp"

#include "raylib.h"

namespace engine {
    namespace core {
        class EventHandler {
            public:
                static bool isKeyPressed(int key);
                static bool isKeyDown(int key);
                static bool isKeyReleased(int key);
                static int getKeyPressed(void);

                static bool isMouseButtonPressed(int button);
                static bool isMouseButtonDown(int button);
                static bool isMouseButtonReleased(int button);
                static Vec2i getMousePosition(void);
        };
    }
}