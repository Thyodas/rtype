/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Draw.cpp
*/

#include "common/ecs/components/Drawable.hpp"
#include "common/ecs/systems/Draw.hpp"
#include "common/ecs/Coordinator.hpp"
#include "common/ecs/components/Physics.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void DrawSystemSFML::handleDraw() {
            for (auto const &entity : _entities) {
                auto& pos = gCoordinator.getComponent<components::physics::position_t>(entity);
                auto& drawable = gCoordinator.getComponent<components::drawable::drawable_sfml_t>(entity);

                if (drawable.isRendered) {
                    drawable.sprite.setPosition({pos.x, pos.y});
                    drawable.window->draw(drawable.sprite);
                }
            }
        }
    }
}