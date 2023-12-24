/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** TestBehaviour.hpp
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/GameEngine.hpp"

#include "my_raylib.h"

class movement : public ecs::components::behaviour::Behaviour {
    public:
        void update(void) override {
            auto &body = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
            auto &transf = _coord->getComponent<ecs::components::physics::transform_t>(_entity);
            if (IsKeyDown(KEY_O)) {
                body.velocity.x += 0.1f;
            }
        };
};
