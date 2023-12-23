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
                    {common::NetworkMessage::serverFireBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onFire(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverDestroyBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDestroy(msg);
                    }},
                });
            }

            // https://www.youtube.com/watch?v=Rd3Uc8Nr0vA
            void onFire(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerFireBullet body;
                auto &bulletBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                this->_speed = body.speed;
                bulletBody.velocity = body.pos;
                engine::rotate(_entity, body.direction);
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerDestroyBullet body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                _coord->destroyEntity(_entity);
            }

            void update() override
            {
                // _speed = 20;
                if (_speed == -1)
                    return;
                auto &bulletBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);

                bulletBody.velocity.z = -_speed;
            }
        private:
            double _speed = -1;
    };
}
