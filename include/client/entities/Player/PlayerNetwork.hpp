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

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit PlayerNetwork(client::NetClient& networkManager, uint32_t netId = 0)
                : NetworkBehaviour(networkManager, netId)
            {
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverUpdateShipPosition, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdatePosition(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverPlayerTakeDamage, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDamageReceive(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverPlayerDestroy, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDestroy(msg);
                    }},
                });
            }

            void onUpdatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipPosition body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                // std::cout << "Player position: " << body.pos.x << ", " << body.pos.y << ", " << body.pos.z << std::endl;

                auto &playerTransform = _coord->getComponent<ecs::components::physics::transform_t>(_entity);

                playerTransform.pos = body.pos;
            }

            void onDamageReceive(rtype::net::Message<common::NetworkMessage>& msg)
            {
                auto &allyHealth = _coord->getComponent<ecs::components::health::health_t>(_entity);
                common::game::netbody::ServerPlayerTakeDamage body;
                msg >> body;

                allyHealth.healthPoints -= body.damage;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerPlayerDestroy body;
                msg >> body;

                _coord->destroyEntity(_entity);
            }

            void updateDirection(const Vector3& direction) const
            {
                _networkManager.reqPingServer();
                rtype::net::Message<common::NetworkMessage> msg;

                msg.header.id = common::NetworkMessage::clientUpdatePlayerDirection;
                common::game::netbody::ClientUpdatePlayerDirection body = {
                    .direction = direction,
                };
                msg << body;
                _networkManager.send(msg);
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
                Vector3 direction = {0};
                if (IsKeyDown(KEY_D)) {
                    // velocity.x += 0.1f;
                    // velocity.z -= 0.1f;
                    direction.z = 1;
                }
                if (IsKeyDown(KEY_A)) {
                    // velocity.x -= 0.1f;
                    // velocity.z += 0.1f;
                    direction.z = -1;
                }
                if (IsKeyDown(KEY_W)) {
                    // velocity.x -= 0.2f;
                    // velocity.z -= 0.2f;
                    direction.y = 1;
                }
                if (IsKeyDown(KEY_S)) {
                    // velocity.x += 0.2f;
                    // velocity.z += 0.2f;
                    direction.y = -1;
                }
                updateDirection(direction);

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