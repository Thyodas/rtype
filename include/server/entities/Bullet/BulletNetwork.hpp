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

#include "common/utils/Math.hpp"

namespace server {

    constexpr float BULLET_SPEED = 2;

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            BulletNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
                _lastUpdate = engine::Engine::getInstance()->getElapsedTime() / 1000;
            }

            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                if (now - _lastUpdate < 1.0 / 60) {
                    return;
                }

                _lastUpdate = now;

                auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(_entity);

                common::game::netbody::ServerUpdateBulletPosition body = {
                    .entityNetId = _entity,
                    .pos = common::utils::joltVectorToRayVector(transform.position),
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverUpdateBulletPosition;
                msg << body;
                //std::cout << "Bullet Pos: " << transform.pos.x << " " << transform.pos.y << " " << transform.pos.z << std::endl;

                _networkManager.messageAllClients(msg);
            }
        private:
            double _lastUpdate = 0;
    };
}
