/*
** EPITECH PROJECT, 2023
** client
** File description:
** PlayerNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "game_engine/ecs/systems/Audio.hpp"
// #include "game_engine/GameEngine.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"
#include "client/entities/Bullet/BulletNetwork.hpp"
#include "common/utils/Math.hpp"

namespace client {

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit PlayerNetwork(client::NetClient& networkManager, uint32_t netId = 0, ecs::SceneID sceneId = 0)
                : NetworkBehaviour(networkManager, netId, 0, sceneId)
            {
            }

            void onAttach(ecs::Entity entity) override
            {
                addResponse({
                    {common::NetworkMessage::serverUpdateShipPosition,
                    [this](rtype::net::Message<common::NetworkMessage> msg)
                    {
                        onUpdatePosition(msg);
                    }},
                    {common::NetworkMessage::serverPlayerTakeDamage,
                    [this](rtype::net::Message<common::NetworkMessage> msg)
                    {
                        onDamageReceive(msg);
                    }},
                    {common::NetworkMessage::serverPlayerDestroy,
                    [this](rtype::net::Message<common::NetworkMessage> msg)
                    {
                        onDestroy(msg);
                    }},
                });
                Sound bulletSound = LoadSound("./ressources/audio/shoot.wav");
                std::function<void(BulletShotEvent&)> fct = [this, bulletSound](BulletShotEvent &event) {
                    if (event.shooter == this->getEntity())
                        engine::triggerAudio(bulletSound);
                };
                addListener<BulletShotEvent>(fct);
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
                common::game::netbody::ServerPlayerTakeDamage body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                auto &allyHealth = _coord->getComponent<ecs::components::health::health_t>(_entity);

                allyHealth.healthPoints -= body.damage;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerPlayerDestroy body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                engine::destroyEntity(_entity);
                unregisterResponses();
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

                if (IsKeyDown(KEY_SPACE) && (engine::Engine::getInstance()->getElapsedTime() / 1000) - _lastBulletFire > 1.0) {
                    BulletShotEvent event(this->_entity);
                    engine::emitEvent<BulletShotEvent>(event);
                    fireBullet();
                    _lastBulletFire = engine::Engine::getInstance()->getElapsedTime() / 1000;
                }
            }
        protected:
            Vector3 _lastDirection{0, 0, 0};
            double _lastBulletFire = 0;
    };
}