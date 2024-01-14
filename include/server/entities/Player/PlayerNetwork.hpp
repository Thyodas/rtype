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
#include "game_engine/core/event/PlayerDestroyEvent.hpp"

#include "common/utils/Math.hpp"

namespace server {

    constexpr float PLAYER_SPEED = 10;

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            PlayerNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0, ecs::SceneID sceneId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId, sceneId)
            {
            }

            void onAttach(ecs::Entity entity) override
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
                    {
                        common::NetworkMessage::clientConnect,
                        [this](std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg) {
                            onClientConnect(client, msg);
                        }
                    },
                });

                addListener<PlayerDestroyEvent>(
                    [this](PlayerDestroyEvent& event) {
                    auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(event.entity);
                    if (metadata.type != server::entities::EntityType::PLAYER)
                        return;

                    engine::destroyEntity(event.entity);
                    unregisterResponses();
                });
            }

            // Inform new players of this player's position
            void onClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                rtype::net::Message<common::NetworkMessage> resMsg;
                resMsg.header.id = common::NetworkMessage::serverAllyConnect;

                auto& transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(_entity);

                //auto &model = engine::Engine::getInstance()->getComponent<ecs::components::Model3D>(ship);

                common::game::netbody::ServerAllyConnect body = {
                    .entityNetId = _entity,
                    .name = "Jean-Michel", // TODO: get name of player
                    .shipName = common::game::ObjectName::DualStriker, // TODO: get ship name from entity
                    .pos =  common::utils::joltVectorToRayVector(transform.position),
                };

                resMsg << body;

                _networkManager.messageClient(client, resMsg);
            }

            void onUpdatePlayerDirection(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ClientUpdatePlayerDirection body;
                msg >> body;

                if (client->getID() != getConnectionId())
                    return;

                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::RigidBodyComponent>(_entity);

                engine::physics::setLinearVelocity(_entity, {
                    PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.x, -1.0f, 1.0f)),
                    PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.y, -1.0f, 1.0f)),
                    PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.z, -1.0f, 1.0f))
                });
                // rigidBody.velocity.SetX(PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.x, -1.0f, 1.0f)));
                // rigidBody.velocity.SetY(PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.y, -1.0f, 1.0f)));
                // rigidBody.velocity.SetZ(PLAYER_SPEED * std::nearbyintf(std::clamp(body.direction.z, -1.0f, 1.0f)));
            }

            void onPlayerFireBullet(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ClientPlayerFireBullet body;
                msg >> body;

                if (client->getID() != getConnectionId())
                    return;

                auto &transf = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(_entity);

                Vector3 newPos = {transf.position.GetX(), transf.position.GetY(), transf.position.GetZ() + 3};
                common::game::EntityFactory factory;
                ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
                    newPos,
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 0, 0},
                    {0.025, 0.025, 0.025}
                }, common::game::ObjectFormat::GLB);
                auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(gunBullet);
                metadata.type = server::entities::EntityType::BULLET;
                auto behave = engine::createBehavior<server::BulletNetwork>(_networkManager, _entity, gunBullet, client->getID(), _sceneID);
                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::RigidBodyComponent>(gunBullet);
                rigidBody.velocity = {0, 0, 5};
                engine::attachBehavior(gunBullet, behave);
                engine::addEntityToScene(gunBullet, _sceneID);
                _networkManager.allServerFireBullet(gunBullet, _entity);
            }

            void update() override
            {
                auto &transf = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(_entity);

                if (transf.position.GetX() != _lastPos.x || transf.position.GetY() != _lastPos.y || transf.position.GetZ() != _lastPos.z) {
                    _networkManager.allServerUpdateShipPosition(_entity);
                    _lastPos = common::utils::joltVectorToRayVector(transf.position);
                }
            }

        protected:
            Vector3 _lastPos = {0, 0, 0};
    };
}
