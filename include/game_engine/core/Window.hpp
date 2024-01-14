/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Window.hpp
*/

#pragma once

#include "my_raylib.h"
#include <iostream>

namespace engine {
    namespace core {
        /**
         * @class Window
         * @brief Represents a window in the graphics engine, providing control
         *  over window properties and rendering.
         *
         * The Window class encapsulates the functionality for creating and managing a window, including its
         * size, fullscreen state, and cursor visibility. It also handles the rendering
         * camera and clipboard text operations.
         */
        class Window {
            public:
                /**
                 * @brief Constructs a new Window object with given dimensions and title.
                 * @param width Width of the window, defaulting to 1920.
                 * @param height Height of the window, defaulting to 1080.
                 * @param name Title of the window, defaulting to "window".
                 */
                Window(int width = 1920, int height = 1000, std::string name = "window");
                ~Window();

                /**
                 * @brief Checks if the window is currently open.
                 * @return True if the window is open, false otherwise.
                 */
                bool isOpen() const;
                /**
                 * @brief Checks if the window is in full screen mode.
                 * @return True if the window is in full screen mode, false otherwise.
                 */
                bool isFullScreen() const;
                /**
                 * @brief Checks if the window is currently hidden.
                 * @return True if the window is hidden, false otherwise.
                 */
                bool isHidden() const;
                /**
                 * @brief Checks if the window has been resized.
                 * @return True if the window has been resized, false otherwise.
                 */
                bool isResized() const;

                /**
                 * @brief Toggles the window between full screen and windowed mode.
                 */
                void toggleFullscreen();

                /**
                 * @brief Gets the width of the window.
                 * @return The width of the window in pixels.
                 */
                int getWidth() const;
                /**
                 * @brief Gets the height of the window.
                 * @return The height of the window in pixels.
                 */
                int getHeight() const;

                /**
                 * @brief Sets the clipboard text to the specified string.
                 * @param text The text to be set to the clipboard.
                 */
                void setClipboardText(std::string text);
                /**
                 * @brief Retrieves the current text from the clipboard.
                 * @return The text currently stored in the clipboard.
                 */
                std::string getClipboardText() const;

                /**
                 * @brief Shows the cursor in the window.
                 */
                void showCursor();
                /**
                 * @brief Hides the cursor in the window.
                 */
                void hideCursor();
                /**
                 * @brief Checks if the cursor is currently hidden.
                 * @return True if the cursor is hidden, false otherwise.
                 */
                bool isCursorHidden() const;

                /**
                 * @brief Clears the window with the specified color.
                 * @param color The color used to clear the window.
                 */
                void clear(Color color);

                /**
                 * @brief Sets the frames per second (FPS) target for the window's rendering.
                 * @param fps The desired FPS rate.
                 */
                void setFPS(int fps);
                /**
                 * @brief Gets the current frames per second (FPS) rate of the window.
                 * @return The current FPS rate.
                 */
                int getFPS() const;

                /**
                 * @brief Gets a reference to the window's camera.
                 * @return A reference to the window's Camera object.
                 */
                Camera &getCamera();
                /**
                 * @brief Sets the camera's position using a Vector3 position.
                 * @param pos The new position of the camera.
                 */
                void setCameraPosition(Vector3 pos);
                /**
                 * @brief Gets the current position of the camera.
                 * @return The current position of the camera as Vector3.
                 */
                Vector3 getCameraPosition() const;
                /**
                 * @brief Sets the camera's target position using a Vector3 position.
                 * @param pos The new target position of the camera.
                 */
                void setCameraTarget(Vector3 pos);
                /**
                 * @brief Gets the current target position of the camera.
                 * @return The current target position of the camera as Vector3.
                 */
                Vector3 getCameraTarget() const;
                /**
                 * @brief Sets the camera's field of view (FoV).
                 * @param fov The field of view angle in degrees.
                 */
                void setCameraFov(float fov);
                /**
                 * @brief Gets the current field of view (FoV) of the camera.
                 * @return The current field of view angle in degrees.
                 */
                float getCameraFov(void) const;

                /**
                 * @brief Gets the view matrix of the camera.
                 * @return The view matrix of the camera.
                 */
                Matrix getCameraViewMatrix() const;

                void setCameraViewMatrix(Matrix matrix);

                /**
                 * @brief Gets the projection matrix of the camera.
                 * @param aspect The aspect ratio of the camera.
                 * @param nearPlane The near plane of the camera.
                 * @param farPlane The far plane of the camera.
                 * @return The projection matrix of the camera.
                 */
                Matrix getProjectionMatrix(double aspect, double nearPlane, double farPlane) const;

            private:
                int _width;
                int _height;
                int _fps;

                Camera _camera;

                std::string _clipboardText;
        };
    }
}
