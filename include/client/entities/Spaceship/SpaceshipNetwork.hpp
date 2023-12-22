/*
** EPITECH PROJECT, 2023
** client
** File description:
** SpaceshipNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "client/core/NetClient.hpp"

namespace client {

    class SpaceshipNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit SpaceshipNetwork(client::NetClient& networkManager)
                : NetworkBehaviour(networkManager)
            {
                _networkManager.registerResponse({
                    {CustomMsgTypes::ServerPing, [this](rtype::net::Message<CustomMsgTypes> msg) {
                        onUpdateVelocity(msg);
                    }},
                });
            }

            void onUpdateVelocity(rtype::net::Message<CustomMsgTypes>& msg)
            {
                // use engine events to register this callback?
                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point timeThen;
                msg >> timeThen;
                std::cout << "Ping from velocity update: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
            }

            void updateVelocity(const Vector3& velocity) const
            {
                auto &body = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                body.velocity.x += velocity.x;
                body.velocity.y += velocity.y;
                body.velocity.z += velocity.z;

                _networkManager.reqPingServer();
            }

            void update() override
            {
                Vector3 velocity = {0};
                if (IsKeyDown(KEY_D)) {
                    velocity.x += 0.1f;
                    velocity.z -= 0.1f;
                }
                if (IsKeyDown(KEY_A)) {
                    velocity.x -= 0.1f;
                    velocity.z += 0.1f;
                }
                if (IsKeyDown(KEY_W)) {
                    velocity.x -= 0.2f;
                    velocity.z -= 0.2f;
                }
                if (IsKeyDown(KEY_S)) {
                    velocity.x += 0.2f;
                    velocity.z += 0.2f;
                }
                if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0)
                    updateVelocity(velocity);
                /*if (IsKeyReleased(KEY_SPACE)) {
                    Vector3 newRotation = {0};
                    newRotation.z = 10 * M_PI / 180;
                    engine::rotate(_entity, newRotation);
                }
                if (IsKeyReleased(KEY_R)) {
                    Vector3 scale = {2, 1, 1};
                    engine::scale(_entity, scale);
                }*/
            }
    };

}