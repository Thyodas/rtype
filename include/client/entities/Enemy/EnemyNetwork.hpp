/*
** EPITECH PROJECT, 2023
** client
** File description:
** EnemyNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"
#include "game_engine/GameEngine.hpp"

namespace client {

    class EnemyNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit EnemyNetwork(client::NetClient& networkManager)
                : NetworkBehaviour(networkManager)
            {
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverUpdateEnemyVelocity, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverEnemyTakeDamage, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDamageReceive(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverDestroyEnemy, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDestroy(msg);
                    }},
                });
            }

            void onUpdateVelocity(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipVelocity body;
                auto &enemyBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                enemyBody.velocity = body.velocity;
            }

            void onDamageReceive(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "Enemy " << _entity << " take damage" << std::endl;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerDestroyEnemy body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                _coord->destroyEntity(_entity);
            }

            void update() override
            {
            }
    };

}
