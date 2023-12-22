/*
** EPITECH PROJECT, 2023
** client
** File description:
** BulletNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"
#include "game_engine/GameEngine.hpp"

namespace client {

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit BulletNetwork(client::NetClient& networkManager)
                : NetworkBehaviour(networkManager)
            {
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverUpdateBulletVelocity, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverBulletTakeDamage, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverDestroyBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
            }

            void onUpdateVelocity(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipVelocity body;
                auto &BulletBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                msg >> body;

                auto &netData = _coord->getComponent<ecs::components::network::network_t>(_entity);

                if (body.entityNetId != netData.entityNetId)
                    return;

                BulletBody.velocity = body.velocity;
            }

            void onDamageReceive(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "Bullet " << _entity << " take damage" << std::endl;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerDestroyBullet body;
                msg >> body;

                auto &netData = _coord->getComponent<ecs::components::network::network_t>(_entity);

                if (body.entityNetId != netData.entityNetId)
                    return;

                _coord->destroyEntity(body.entityNetId);
            }

            void update() override
            {
            }
    };

}
