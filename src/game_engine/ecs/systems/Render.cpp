/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.cpp
*/

#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Coordinator.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void RenderSystem::render(ecs::Coordinator &coord) {
            for (auto const &entity : _entities) {
                auto& transf = coord.getComponent<components::physics::transform_t>(entity);
                auto& render = coord.getComponent<components::render::render_t>(entity);

                if (render.isRendered)
                    render.data->draw(transf);
            }
        }
    }
}