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

namespace client {

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit BulletNetwork(client::NetClient& networkManager, uint32_t netId = 0)
                : NetworkBehaviour(networkManager, netId)
            {
                _networkManager.registerResponse({
                    {
                        common::NetworkMessage::serverDestroyBullet,
                        [this](rtype::net::Message<common::NetworkMessage> msg) {
                            onDestroy(msg);
                        }
                    },
                    {
                        common::NetworkMessage::serverUpdateBulletPosition,
                        [this](rtype::net::Message<common::NetworkMessage> msg) {
                            updatePosition(msg);
                        }
                    },
                });
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerDestroyBullet body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                _coord->destroyEntity(_entity);
            }

            void updatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateBulletPosition body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_entity);
                transform.pos = body.pos;
            }

            void update() override
            {
            }

        private:
            double _lastUpdate = 0;
    };
}
