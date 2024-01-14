/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Lights.hpp
*/

#pragma once

#include "my_raylib.h"

#include "../System.hpp"
#include "../Coordinator.hpp"

#include "../components/Lights.hpp"

namespace ecs::system {

    class LightSystem : public System {
    public:
        void update()
        {
            for (auto& entity : _entities) {
                auto& light = _coord->getComponent<components::lights::LightSource>(entity);
                DrawLight(light);
            }
        }

    private:
        void DrawLight(const components::lights::LightSource& light)
        {
            DrawCircleV(light.position, light.intensity, light.color);
        }
    };

}