/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Window.cpp
*/

#include "Window.hpp"

namespace engine {
    namespace core {
        Window::Window(int width, int height, std::string name)
        {
            _width = width;
            _height = height;
            _camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
            _camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
            _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
            _camera.fovy = 45.0f;
            _camera.projection = CAMERA_FREE;
            InitWindow(width, height, name.c_str());
        }

        Window::~Window()
        {
            CloseWindow();
        }

        bool Window::isOpen() const
        {
            return !WindowShouldClose();
        }

        bool Window::isFullScreen() const
        {
            return IsWindowFullscreen();
        }

        bool Window::isHidden() const
        {
            return IsWindowFullscreen();
        }

        bool Window::isResized() const
        {
            return IsWindowResized();
        }

        void Window::toggleFullscreen()
        {
            return ToggleFullscreen();
        }

        int Window::getWidth() const
        {
            return _width;
        }

        int Window::getHeight() const
        {
            return _height;
        }

        void Window::setClipboardText(std::string text)
        {
            _clipboardText = text;
        }

        std::string Window::getClipboardText() const
        {
            return _clipboardText;
        }

        void Window::showCursor()
        {
            ShowCursor();
        }

        void Window::hideCursor()
        {
            HideCursor();
        }

        bool Window::isCursorHidden() const
        {
            return IsCursorHidden();
        }

        void Window::clear(Color color)
        {
            ClearBackground(color);
        }

        void Window::setFPS(int fps)
        {
            _fps = fps;
            SetTargetFPS(fps);
        }

        int Window::getFPS() const
        {
            return _fps;
        }

        Camera &Window::getCamera(void)
        {
            return _camera;
        }

        void Window::setCameraPosition(float x, float y)
        {
            _camera.position.x = x;
            _camera.position.y = y;
        }

        void Window::setCameraPosition(Vec2i pos)
        {
            _camera.position.x = pos.x;
            _camera.position.y = pos.y;
        }

        Vec2f Window::getCameraPosition() const
        {
            return Vec2f(_camera.position.x, _camera.position.y);
        }

        void Window::setCameraTarget(float x, float y)
        {
            _camera.target.x = x;
            _camera.target.y = y;
        }

        void Window::setCameraTarget(Vec2f pos)
        {
            _camera.target.x = pos.x;
            _camera.target.y = pos.y;
        }

        Vec2f Window::getCameraTarget() const
        {
            return Vec2f(_camera.target.x, _camera.target.y);
        }

        void Window::setCameraFov(float fov)
        {
            _camera.fovy = fov;
        }

        float Window::getCameraFov(void) const
        {
            return _camera.fovy;
        }
    }
}