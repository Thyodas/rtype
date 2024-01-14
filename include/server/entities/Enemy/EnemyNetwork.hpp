/*
** EPITECH PROJECT, 2023
** client
** File description:
** EnemyNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "server/core/NetServer.hpp"
#include "server/entities/Bullet/BulletNetwork.hpp"

namespace server {

    class EnemyNetwork : public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            EnemyNetwork(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0, ecs::SceneID sceneId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId, sceneId)
            {
                _lastShot = engine::Engine::getInstance()->getElapsedTime() / 1000;
            }

            void shoot()
            {
                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;

                if (now - _lastShot < 5) {
                    return;
                }

                _lastShot = now;

                std::cout << "shooting" << std::endl;

                auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_entity);
                common::game::EntityFactory factory;
                ecs::Entity bullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
                    {transform.pos.x, transform.pos.y, transform.pos.z},
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 135, 0},
                    {0.025, 0.025, 0.025}
                }, common::game::ObjectFormat::GLB);
                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(bullet);
                rigidBody.velocity = {0, 0, -2};
                auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(bullet);
                metadata.type = server::entities::EntityType::BULLET;
                auto bulletBehave = engine::createBehavior<server::BulletNetwork>(_networkManager, _entity, bullet, 0, _sceneID);
                engine::attachBehavior(bullet, bulletBehave);
                engine::addEntityToScene(bullet, _sceneID);

                _networkManager.allServerFireBullet(bullet, _entity);
            }

            void update() override
            {
                // this->shoot();
            }

        private:
            double _lastShot = 0;
    };

}
