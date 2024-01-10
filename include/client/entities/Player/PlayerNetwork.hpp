/*
** EPITECH PROJECT, 2023
** client
** File description:
** PlayerNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "game_engine/ecs/systems/Audio.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"

namespace client {

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        ecs::system::AudioSystem _audioSystem;
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
                _audioSystem.initialize();
                _audioSystem.addSound("../../ressources/audio/shoot.wav", "PlayerShoot");
            }


            // void onFire(rtype::net::Message<common::NetworkMessage>& msg)
            // {
            //     std::cout << "received fire bullet from server" << std::endl;
            //     common::game::netbody::ServerFireBullet body;
            //     msg >> body;

            //     //std::cout << "bullet pos: " << body.pos.x << " " << body.pos.y << " " << body.pos.z << std::endl;
            //     common::game::EntityFactory factory;
            //     ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
            //         body.pos,
            //         0,
            //         0,
            //         0,
            //         WHITE,
            //         false,
            //         WHITE,
            //         {0, 0, 0},
            //         {0.025, 0.025, 0.025}
            //     }, common::game::ObjectFormat::GLB);

            //     auto &direction = engine::Engine::getInstance()->getComponent<ecs::components::direction::direction_t>(gunBullet);
            //     direction.direction = body.direction;
            //     auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(gunBullet);
            //     // rigidBody.velocity = {0, 0, static_cast<float>(body.speed)};
            //     rigidBody.velocity = { 0, 0, 0};

            //     auto behave = engine::createBehavior<client::BulletNetwork>(_networkManager, body.entityNetId);
            //     engine::attachBehavior(gunBullet, behave);
            // }

            void onUpdatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipPosition body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

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
                msg << body;
                //std::cout << "move: " << direction.x << direction.y << direction.z  << msg << std::endl;
                _networkManager.send(msg);
            }

            void fireBullet()
            {
                auto &trans = _coord->getComponent<ecs::components::physics::transform_t>(_entity);

                // Vector3 velocity = calculateBulletVelocity(trans, 10);

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::clientPlayerFireBullet;
                common::game::netbody::ClientPlayerFireBullet body = {
                    .direction = {0, 0, 1},
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

                if (IsKeyDown(KEY_SPACE) && (engine::Engine::getInstance()->getElapsedTime() / 1000) - _lastBulletFire > 1.0) {
                    std::cout << "PRESSED SPACE -> FIRE BULLET" << std::endl;
                    _audioSystem.playSound("PlayerShoot");
                    fireBullet();
                    _lastBulletFire = engine::Engine::getInstance()->getElapsedTime() / 1000;
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
        protected:
            Vector3 _lastDirection{0, 0, 0};
            double _lastBulletFire = 0;
    };
}