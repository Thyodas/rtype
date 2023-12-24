/*
** EPITECH PROJECT, 2023
** client
** File description:
** PlayerNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "server/core/NetServer.hpp"
#include "common/game/entities/EntityFactory.hpp"
#include "server/entities/Bullet/BulletNetwork.hpp"

namespace server {

    constexpr float PLAYER_SPEED = 10;

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            PlayerNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
                _networkManager.registerResponse({
                    {
                        common::NetworkMessage::clientUpdatePlayerDirection,
                        [this](std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg) {
                            onUpdatePlayerDirection(client, msg);
                        }
                    },
                    {
                        common::NetworkMessage::clientPlayerFireBullet,
                        [this](std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg) {
                            onPlayerFireBullet(client, msg);
                        }
                    },
                });
            }

            void onUpdatePlayerDirection(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ClientUpdatePlayerDirection body;
                msg >> body;

                if (client->getID() != getConnectionId())
                    return;

                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(_entity);

                rigidBody.velocity.x = PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.x, -1.0f, 1.0f));
                rigidBody.velocity.y = PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.y, -1.0f, 1.0f));
                rigidBody.velocity.z = PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.z, -1.0f, 1.0f));
            }

            void onPlayerFireBullet(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "received fire bullet from client" << std::endl;
                common::game::netbody::ClientPlayerFireBullet body;
                msg >> body;

                auto &transf = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_entity);

                common::game::EntityFactory factory;
                ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
                    transf.pos,
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
                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(gunBullet);
                rigidBody.velocity = {0, 0, BULLET_SPEED};
                auto behave = engine::createBehavior<server::BulletNetwork>(_networkManager, gunBullet);
                engine::attachBehavior(gunBullet, behave);
                _networkManager.allServerFireBullet(gunBullet);
            }

            void update() override
            {
                auto &transf = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_entity);

                if (transf.pos.x != _lastPos.x || transf.pos.y != _lastPos.y || transf.pos.z != _lastPos.z) {
                    _networkManager.allServerUpdateShipPosition(_entity);
                    _lastPos = transf.pos;
                }
            }

        protected:
            Vector3 _lastPos = {0, 0, 0};
    };
}
