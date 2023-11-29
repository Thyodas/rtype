/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Controls.cpp
*/

#include "common/ecs/Coordinator.hpp"
#include "common/ecs/systems/Controls.hpp"
#include "common/ecs/components/Controls.hpp"
#include "common/ecs/components/Physics.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void ControlSystem::updateControl() {
            for (auto const &entity : _entities) {
                auto& vel = gCoordinator.getComponent<ecs::components::physics::velocity_t>(entity);
                auto &ctr = gCoordinator.getComponent<components::controls::controllable_t>(entity);

                vel.x = (ctr.isMovingRight - ctr.isMovingLeft) * 10;
                vel.y = (ctr.isMovingDown - ctr.isMovingUp) * 10;
                ctr.isMovingRight = false;
                ctr.isMovingLeft = false;
                ctr.isMovingDown = false;
                ctr.isMovingUp = false;
            }
        }

        void ControlSystem::updateInputs() {
            for (auto const &entity : _entities) {
                auto &ctr = gCoordinator.getComponent<components::controls::controllable_t>(entity);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                    ctr.isMovingUp = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    ctr.isMovingLeft = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    ctr.isMovingDown = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    ctr.isMovingRight = true;
            }
        }
    }
}

