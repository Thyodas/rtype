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
#include "game_engine/GameEngine.hpp"
#include "game_engine/ecs/components/Physics.hpp"

namespace server {

    constexpr float BULLET_SPEED = 2;
    constexpr float BULLET_TTL = 10;

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            BulletNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                _lastUpdate = now;
                _spawnedAt = now;

                _coord->registerListener<CollisionEvent>([this](const CollisionEvent &event) {
                    // std::cout << "Bullet collides with " << event.entity2 << std::endl;
                });
            }

            bool verifyBulletExpire()
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                if (now - _spawnedAt > BULLET_TTL) {
                    common::game::netbody::ServerDestroyBullet body = {
                        .entityNetId = _entity,
                    };

                    rtype::net::Message<common::NetworkMessage> msg;
                    msg.header.id = common::NetworkMessage::serverDestroyBullet;
                    msg << body;

                    _networkManager.messageAllClients(msg);
                    std::cout << "destroyed bullet" << std::endl;
                    return true;
                }
                return false;
            }

            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                // if (verifyBulletExpire()) {
                //     std::cout << "before" << std::endl;
                //     _coord->destroyEntity(_entity);
                //     std::cout << "after" << std::endl;
                //     return;
                // }

                if (now - _lastUpdate < 1.0 / 60) {
                    return;
                }

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
            double _spawnedAt = 0;
    };
}
