/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** TestBehaviour.hpp
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"

#include "raylib.h"

class input : public ecs::components::behaviour::Behaviour {
    public:
        void update(void) override {
            auto &body = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
            if (IsKeyDown(KEY_D)) {
                body.velocity.x += 0.1f;
                body.velocity.z += -0.1f;
            }
            if (IsKeyDown(KEY_A)) {
                body.velocity.x += -0.1f;
                body.velocity.z += 0.1f;
            }
            if (IsKeyDown(KEY_W)) {
                body.velocity.x += -0.2f;
                body.velocity.z += -0.2f;
            }
            if (IsKeyDown(KEY_S)) {
                body.velocity.x += 0.2f;
                body.velocity.z += 0.2f;
            }
        };
};
