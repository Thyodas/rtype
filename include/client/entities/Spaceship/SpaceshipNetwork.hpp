/*
** EPITECH PROJECT, 2023
** client
** File description:
** SpaceshipNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "client/core/NetClient.hpp"

namespace client {

    class SpaceshipNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit SpaceshipNetwork(client::NetClient& networkManager)
                : NetworkBehaviour(networkManager)
            {
                _networkManager.registerResponse({
                    {common::NetworkMessage::ServerPing, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
            }



            void onUpdateVelocity(rtype::net::Message<common::NetworkMessage>& msg)
            {
                // use engine events to register this callback?
                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point timeThen;
                msg >> timeThen;


                std::cout << "Ping from velocity update: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
            }

            void updateVelocity(const Vector3& velocity) const
            {
                auto &body = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                body.velocity.x += velocity.x;
                body.velocity.y += velocity.y;
                body.velocity.z += velocity.z;

                _networkManager.reqPingServer();
            }

            Vector3 calculateBulletVelocity(const ecs::components::physics::transform_t& shipTransform, float bulletSpeed)
            {
                // Assuming the ship's forward direction is the negative z-axis
                Vector3 shipForward = shipTransform.rotation;

                // Negate the z-axis to get the forward direction
                shipForward.z = -shipForward.z;

                // Calculate the bullet velocity
                Vector3 bulletVelocity = Vector3Scale(shipForward, bulletSpeed);

                return bulletVelocity;
            }

            void fireBullet()
            {
                auto &trans = _coord->getComponent<ecs::components::physics::transform_t>(_entity);

                Vector3 velocity = calculateBulletVelocity(trans, 10);

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::clientPlayerFireBullet;

                msg << velocity;
                msg << trans.pos;
                _networkManager.send(msg);
            }

            void update() override
            {
                Vector3 velocity = {0};
                if (IsKeyDown(KEY_D)) {
                    velocity.x += 0.1f;
                    velocity.z -= 0.1f;
                }
                if (IsKeyDown(KEY_A)) {
                    velocity.x -= 0.1f;
                    velocity.z += 0.1f;
                }
                if (IsKeyDown(KEY_W)) {
                    velocity.x -= 0.2f;
                    velocity.z -= 0.2f;
                }
                if (IsKeyDown(KEY_S)) {
                    velocity.x += 0.2f;
                    velocity.z += 0.2f;
                }
                if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0)
                    updateVelocity(velocity);

                if (IsKeyDown(KEY_SPACE)) {
                    fireBullet();
                }
                /*if (IsKeyReleased(KEY_SPACE)) {
                    Vector3 newRotation = {0};
                    newRotation.z = 10 * M_PI / 180;
                    engine::rotate(_entity, newRotation);
                }
                if (IsKeyReleased(KEY_R)) {
                    Vector3 scale = {2, 1, 1};
                    engine::scale(_entity, scale);
                }*/
            }
    };

}