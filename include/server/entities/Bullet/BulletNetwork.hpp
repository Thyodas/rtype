/*
** EPITECH PROJECT, 2023
** client
** File description:
** BulletNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "server/core/NetServer.hpp"

namespace server {

    constexpr float BULLET_SPEED = 2;

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            BulletNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
            }

            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                if (now - _lastUpdate < 1.0 / 60.0)
                    return;

                _lastUpdate = now;

                auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_entity);

                common::game::netbody::ServerUpdateBulletPosition body = {
                    .entityNetId = _entity,
                    .pos = transform.pos,
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverUpdateBulletPosition;
                msg << body;

                _networkManager.messageAllClients(msg);
            }
        private:
            double _lastUpdate = 0;
    };
}
