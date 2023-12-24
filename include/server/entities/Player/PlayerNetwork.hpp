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

    class PlayerNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            PlayerNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
                _networkManager.registerResponse({
                    {common::NetworkMessage::clientUpdatePlayerDirection, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        serverClientUpdatePlayerDirection(msg);
                    }},
                });
            }

            void update() override
            {

            }

            void serverClientUpdatePlayerDirection(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ClientUpdatePlayerDirection body;
                msg >> body;

                ecs::Entity player = getNetId();

                if (body.direction.x > 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.x += 1;
                } else if (body.direction.x < 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.x -= 1;
                }
                if (body.direction.y > 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.y += 1;
                } else if (body.direction.y < 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.y -= 1;
                }
                if (body.direction.z > 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.z += 1;
                } else if (body.direction.z < 0) {
                    engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity.z -= 1;
                }

                rtype::net::Message<common::NetworkMessage> msg2;
                msg.header.id = common::NetworkMessage::serverUpdateShipPosition;

                common::game::netbody::ServerUpdateShipPosition body2 = {
                    .entityNetId = player,
                    .pos = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity,
                };

                msg2 << body2;
                _networkManager.allServerUpdatePlayerShipPosition(player, engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(player).velocity);
            }
    };

}