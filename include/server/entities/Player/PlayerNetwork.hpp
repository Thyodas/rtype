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
                });
            }

            void onUpdatePlayerDirection(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ClientUpdatePlayerDirection body;
                msg >> body;

                if (client->getID() != getConnectionId())
                    return;

                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(_entity);

                if (body.direction.x > 0) {
                    rigidBody.velocity.x = 1 * PLAYER_SPEED;
                } else if (body.direction.x < 0) {
                    rigidBody.velocity.x = -1 * PLAYER_SPEED;
                }
                if (body.direction.y > 0) {
                    rigidBody.velocity.y = 1 * PLAYER_SPEED;
                } else if (body.direction.y < 0) {
                    rigidBody.velocity.y = -1 * PLAYER_SPEED;
                }
                if (body.direction.z > 0) {
                    rigidBody.velocity.z = 1 * PLAYER_SPEED;
                } else if (body.direction.z < 0) {
                    rigidBody.velocity.z = -1 * PLAYER_SPEED;
                }

                //std::cout << "Player " << getNetId() << " direction: " << rigidBody.velocity.x << ", " << rigidBody.velocity.y << ", " << rigidBody.velocity.z << std::endl;
            }

            void update() override
            {
                _networkManager.allServerUpdateShipPosition(_entity);
            }


    };

}