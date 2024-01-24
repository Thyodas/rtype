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
#include "game_engine/core/event/EnemyDestroyEvent.hpp"
#include "game_engine/core/event/PlayerDestroyEvent.hpp"

namespace server {

    constexpr float BULLET_SPEED = 4;
    constexpr float BULLET_TTL = 10.0;

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            BulletNetwork(server::NetServer& networkManager, uint32_t sender, uint32_t entityNetId = 0, uint32_t connectionId = 0, ecs::SceneID sceneId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId, sceneId)
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

                    auto &metadata1 = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(event.entity2);
                    if (metadata1.type != server::entities::EntityType::BULLET)
                        return;

                    auto &life = engine::Engine::getInstance()->getComponent<ecs::components::health::health_t>(event.entity1);
                    if (verifyBulletCollision(event.entity1, event.entity2)) return;
                    engine::destroyEntity(_entity);
                    destroyBullet(_entity);

                    if ((int)life.healthPoints - 30 < 0) {
                        auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(event.entity1);
                        if (metadata.type == server::entities::EntityType::ENEMY) {
                            EnemyDestroyEvent evt(event.entity1);
                            engine::emitEvent<EnemyDestroyEvent>(evt);
                            destroyEnemy(event.entity1);
                            engine::destroyEntity(event.entity1);
                        } else if (metadata.type == server::entities::EntityType::PLAYER) {
                            notifyAlliesDestroy(event.entity1, event.entity1);
                            notifyPlayerDestroy(event.entity1);
                            PlayerDestroyEvent evt(event.entity1);
                            engine::emitEvent<PlayerDestroyEvent>(evt);
                        }
                        return;
                    }

                    life.healthPoints -= 30;
                });
            }

            bool verifyBulletCollision(ecs::Entity entity1, ecs::Entity entity2)
            {
                auto &metadata1 = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(entity1);
                auto &metadata2 = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(entity2);

                if (metadata1.type == server::entities::EntityType::BULLET && metadata2.type == server::entities::EntityType::BULLET) {
                    engine::destroyEntity(entity1);
                    engine::destroyEntity(entity2);
                    destroyBullet(entity1);
                    destroyBullet(entity2);
                    return true;
                }
                return false;
            }

            void notifyPlayerDestroy(ecs::Entity id)
            {
                auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(id);
                common::game::netbody::ServerPlayerDestroy body = {
                    .entityNetId = id,
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverPlayerDestroy;
                msg << body;

                _networkManager.messageClient(metadata.client, msg);
            }

            void notifyAlliesDestroy(ecs::Entity id, ecs::Entity playerToDestroy)
            {
                auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(playerToDestroy);
                common::game::netbody::ServerAllyDestroy body = {
                    .entityNetId = id,
                };

                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::serverAllyDestroy;
                msg << body;

                _networkManager.messageAllClients(msg, metadata.client);
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

            void destroyBullet(ecs::Entity id)
            {
                common::game::netbody::ServerDestroyBullet body = {
                    .entityNetId = id,
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
                    return true;
                }
                return false;
            }

            void update() override
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                if (verifyBulletExpire()) {
                    destroyBullet(_entity);
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
            ecs::Entity _sender;
    };
}
