/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Camera.cpp
*/

#include "game_engine/core/Camera.hpp"

namespace engine {
    namespace core {
        EngineCamera::EngineCamera(Vector3 position, Vector3 target, Vector3 up, int mode, float fov)
        {
            _camera.position = position;
            _camera.target = target;
            _camera.up = up;
            _camera.projection = mode;
            _camera.fovy = fov;
        }
    }
}