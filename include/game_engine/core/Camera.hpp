/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Camera.hpp
*/

#pragma once

#include "raylib.h"

namespace engine {
    namespace core {
        /**
         * @class Camera
         * @brief Represents a camera in the graphics engine, providing control
         *  over camera properties and rendering.
         *
         * The Camera class encapsulates the functionality for creating and managing a camera, including its
         * position, target, and up vector. It also handles the rendering
         * camera and clipboard text operations.
         */
        class EngineCamera {
            public:
                /**
                 * @brief Constructs a new Camera object with given position, target, and up vector.
                 * @param position Position of the camera, defaulting to {0, 0, 0}.
                 * @param target Target of the camera, defaulting to {0, 0, 0}.
                 * @param up Up vector of the camera, defaulting to {0, 0, 0}.
                 */
                EngineCamera(
                    Vector3 position = {0, 0, 0},
                    Vector3 target = {0, 0, 0},
                    Vector3 up = {0, 0, 0},
                    int mode = CAMERA_PERSPECTIVE,
                    float fov = 45.0f)
                {
                    _camera.position = position;
                    _camera.target = target;
                    _camera.up = up;
                    _camera.projection = mode;
                    _camera.fovy = fov;
                };
                ~EngineCamera() {};

                /**
                 * @brief Sets the position of the camera.
                 * @param position Position of the camera.
                 */
                void setPosition(Vector3 position)
                {
                    _camera.position = position;
                };
                /**
                 * @brief Sets the target of the camera.
                 * @param target Target of the camera.
                 */
                void setTarget(Vector3 target)
                {
                    _camera.target = target;
                };
                /**
                 * @brief Sets the up vector of the camera.
                 * @param up Up vector of the camera.
                 */
                void setUp(Vector3 up)
                {
                    _camera.up = up;
                };

                /**
                 * @brief Gets the position of the camera.
                 * @return Position of the camera.
                 */
                Vector3 getPosition() const
                {
                    return _camera.position;
                };
                /**
                 * @brief Gets the target of the camera.
                 * @return Target of the camera.
                 */
                Vector3 getTarget() const
                {
                    return _camera.target;
                };
                /**
                 * @brief Gets the up vector of the camera.
                 * @return Up vector of the camera.
                 */
                Vector3 getUp() const
                {
                    return _camera.up;
                };

                Camera getCamera() const
                {
                    return _camera;
                }

                RenderTexture &getRenderTexture()
                {
                    return _viewTexture;
                }
            private:
                Camera _camera;
                RenderTexture _viewTexture;
        };
    }
}