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
#include "client/entities/Bullet/BulletNetwork.hpp"
#include "common/utils/Math.hpp"

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
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverFireBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onFire(msg);
                    }},
                });
            }

            void onFire(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "received fire bullet from server" << std::endl;
                common::game::netbody::ServerFireBullet body;
                msg >> body;

                //std::cout << "bullet pos: " << body.pos.x << " " << body.pos.y << " " << body.pos.z << std::endl;
                common::game::EntityFactory factory;
                ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
                    body.pos,
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 0, 0},
                    {0.025, 0.025, 0.025}
                }, common::game::ObjectFormat::GLB);

                auto &direction = engine::Engine::getInstance()->getComponent<ecs::components::direction::direction_t>(gunBullet);
                direction.direction = body.direction;
                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::RigidBodyComponent>(gunBullet);
                // rigidBody.velocity = {0, 0, static_cast<float>(body.speed)};
                rigidBody.velocity = { 0, 0, 0};

                auto behave = engine::createBehavior<client::BulletNetwork>(_networkManager, body.entityNetId);
                engine::attachBehavior(gunBullet, behave);
            }

            void onUpdatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipPosition body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                auto &playerTransform = _coord->getComponent<ecs::components::physics::TransformComponent>(_entity);

                playerTransform.position = common::utils::rayVectorToJoltVector(body.pos);
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

            void updateDirectionOnChange(const Vector3& direction)
            {
                if (_lastDirection.x == direction.x && _lastDirection.y == direction.y && _lastDirection.z == direction.z)
                    return;
                _lastDirection = direction;
                rtype::net::Message<common::NetworkMessage> msg;

                msg.header.id = common::NetworkMessage::clientUpdatePlayerDirection;
                common::game::netbody::ClientUpdatePlayerDirection body = {
                    .direction = direction,
                };
                std::cout << "on envoie la nouvelle direction" << std::endl;
                msg << body;
                _networkManager.send(msg);
            }

            void fireBullet()
            {
                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::clientPlayerFireBullet;
                common::game::netbody::ClientPlayerFireBullet body = {
                    .direction = {0, 0, 1},
                };
                msg << body;
                _networkManager.send(msg);
            }

            void update() override
            {
                Vector3 direction = {0, 0, 0};
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
                updateDirectionOnChange(direction);

                if (IsKeyDown(KEY_SPACE)) {
                    std::cout << "PRESSED SPACE -> FIRE BULLET" << std::endl;
                    fireBullet();
                }
            }
        protected:
            Vector3 _lastDirection{0, 0, 0};
    };
}