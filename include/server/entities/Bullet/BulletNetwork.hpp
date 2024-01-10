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

    constexpr float BULLET_SPEED = 4;
    constexpr float BULLET_TTL = 10.0;

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            BulletNetwork(server::NetServer& networkManager, ecs::Entity sender, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                _lastUpdate = now;
                _spawnedAt = now;
                _sender = sender;
            }

            void onAttach(ecs::Entity entity) override
            {
                addListener<CollisionEvent>([this](CollisionEvent &event) {
                    if (event.entity1 == _sender || event.entity2 == _sender) {
                        return;
                    }
                    if (event.entity1 != _entity && event.entity2 != _entity)
                        return;
                    auto &life = engine::Engine::getInstance()->getComponent<ecs::components::health::health_t>(event.entity1);
                    destroyBullet();
                    engine::destroyEntity(_entity);

                    if ((int)life.healthPoints - 30 < 0) {
                        destroyEnemy(event.entity1);
                        engine::destroyEntity(event.entity1);
                        return;
                    }

                    life.healthPoints -= 30;
                });
            }

            void destroyEnemy(ecs::Entity entity)
            {
                common::game::netbody::ServerDestroyEnemy body = {
                    .entityNetId = entity,
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverDestroyEnemy;
                msg << body;

                _networkManager.messageAllClients(msg);
            }

            void destroyBullet()
            {
                common::game::netbody::ServerDestroyBullet body = {
                    .entityNetId = _entity,
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverDestroyBullet;
                msg << body;

                _networkManager.messageAllClients(msg);
            }

            bool verifyBulletExpire()
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                if (now - _spawnedAt > BULLET_TTL) {
                    this->destroyBullet();
                    return true;
                }
                return false;
            }

            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                if (verifyBulletExpire()) {
                    destroyBullet();
                    engine::destroyEntity(_entity);
                    return;
                }

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
            // save the origin
            ecs::Entity _sender;
    };
}
