/*
** EPITECH PROJECT, 2023
** client
** File description:
** PlayerNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"

namespace client {

    class SkyboxBehavior : public ecs::components::behaviour::Behaviour {
        public:
            Vector3 MatrixToEuler(Matrix mat) {
                Vector3 euler;

                float m12 = mat.m6;
                float m02 = mat.m8;
                float m22 = mat.m10;
                float m10 = mat.m1;
                float m11 = mat.m5;
                float m01 = mat.m4;
                float m00 = mat.m0;

                if (m12 != 1.0f && m12 != -1.0f) {
                    euler.x = -asin(m12);
                    euler.y = atan2(m02 / cos(euler.x), m22 / cos(euler.x));
                    euler.z = atan2(m10 / cos(euler.x), m11 / cos(euler.x));
                } else {
                    euler.z = 0;
                    if (m12 == -1.0f) {
                        euler.x = PI / 2;
                        euler.y = atan2(m01, m00);
                    } else {
                        euler.x = -PI / 2;
                        euler.y = atan2(-m01, -m00);
                    }
                }

                return euler;
            }
            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                float rotationAngle = rotationSpeed * (float)(now - _lastTime);

                Matrix rotation = MatrixRotateXYZ(Vector3{ 0, DEG2RAD * rotationAngle, 0 });
                Vector3 test = MatrixToEuler(rotation);
                engine::rotate(_entity, test);
                _lastTime = now;
            }
        private:
            double _lastTime = 0;
            float rotationSpeed = 100.0f;
    };
}
