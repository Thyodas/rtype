/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Event.cpp
*/

#include "Event.hpp"

namespace engine {
    namespace core {
        bool EventHandler::isKeyPressed(int key)
        {
            return IsKeyPressed(key);
        }

        bool EventHandler::isKeyDown(int key)
        {
            return IsKeyDown(key);
        }

        bool EventHandler::isKeyReleased(int key)
        {
            return IsKeyReleased(key);
        }

        int EventHandler::getKeyPressed(void)
        {
            return GetKeyPressed();
        }

        bool EventHandler::isMouseButtonPressed(int button)
        {
            return IsMouseButtonPressed(button);
        }

        bool EventHandler::isMouseButtonDown(int button)
        {
            return IsMouseButtonDown(button);
        }

        bool EventHandler::isMouseButtonReleased(int button)
        {
            return IsMouseButtonReleased(button);
        }

        Vec2i EventHandler::getMousePosition(void)
        {
            Vector2 pos = GetMousePosition();
            return Vec2i(pos.x, pos.y);
        }
    }
}