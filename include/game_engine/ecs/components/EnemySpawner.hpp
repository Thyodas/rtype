/*
** EPITECH PROJECT, 2023
** server
** File description:
** EnemySpawner.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "server/core/NetServer.hpp"
#include "common/game/entities/EntityFactory.hpp"
#include "server/entities/Bullet/BulletNetwork.hpp"

namespace ecs::components::behaviour
{
    class EnemySpawner: public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
    // class EnemySpawner : public Behaviour {
        public:
            explicit EnemySpawner(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId) {}

            void addEnemy()
            {
                common::game::EntityFactory factory;
                ecs::Entity entity = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::Transtellar, {
                    {0, 0, 10},
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 180, 0},
                    {1, 1, 1}
                }, common::game::ObjectFormat::OBJ);

                rtype::net::Message<common::NetworkMessage> resMsg;
                resMsg.header.id = common::NetworkMessage::serverCreateEnemy;
                common::game::netbody::ServerCreateEnemy body = {
                    .entityNetId = entity,
                    .name = "Transtellar",
                    .shipName = common::game::ObjectName::Transtellar,
                    .pos = {0, 0, 10},
                };
               resMsg << body;
               std::cout << "creating enemy" << std::endl;
                _networkManager.messageAllClients(resMsg);

                _ennemies.emplace(entity);
            }

            void removeEnemy(Entity entity)
            {
                _ennemies.erase(entity);
            }

            void update()
            {
                if (_ennemies.size() == 0) {
                    addEnemy();
                }
            }
        private:
            std::set<Entity> _ennemies;
    };
}
