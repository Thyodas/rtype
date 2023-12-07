/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Window.hpp
*/

#pragma once

#include "raylib.h"
#include "common/math/vectors/Vec2.hpp"
#include <iostream>

namespace engine {
    namespace core {
        class Window {
            public:
                Window(int width = 1920, int height = 1080, std::string name = "window");
                ~Window();

                bool isOpen() const;
                bool isFullScreen() const;
                bool isHidden() const;
                bool isResized() const;

                void toggleFullscreen();

                int getWidth() const;
                int getHeight() const;

                void setClipboardText(std::string text);
                std::string getClipboardText() const;

                void showCursor();
                void hideCursor();
                bool isCursorHidden() const;

                void clear(Color color);

                void setFPS(int fps);
                int getFPS() const;

                Camera &getCamera();
                void setCameraPosition(float x, float y);
                void setCameraPosition(Vec2i pos);
                Vec2f getCameraPosition() const;
                void setCameraTarget(float x, float y);
                void setCameraTarget(Vec2f pos);
                Vec2f getCameraTarget() const;
                void setCameraFov(float fov);
                float getCameraFov(void) const;

            private:
                int _width;
                int _height;
                int _fps;

                Camera _camera;

                std::string _clipboardText;
        };
    }
}