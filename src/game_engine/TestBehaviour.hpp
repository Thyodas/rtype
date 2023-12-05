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
        void update(ecs::components::physics::transform_t &transf) override {
            if (IsKeyDown(KEY_D)) {
                transf.pos.x += 0.1f;
                transf.pos.z -= 0.1f;
            }
            if (IsKeyDown(KEY_A)) {
                transf.pos.x -= 0.1f;
                transf.pos.z += 0.1f;
            }
            if (IsKeyDown(KEY_W)) {
                transf.pos.x -= 0.2f;
                transf.pos.z -= 0.2f;
            }
            if (IsKeyDown(KEY_S)) {
                transf.pos.x += 0.2f;
                transf.pos.z += 0.2f;
            }
        };
};
