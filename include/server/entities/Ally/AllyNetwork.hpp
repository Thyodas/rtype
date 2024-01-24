/*
** EPITECH PROJECT, 2023
** client
** File description:
** AllyNetwork.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "client/core/NetClient.hpp"

namespace client {

    class AllyNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit AllyNetwork(client::NetClient& networkManager)
                : NetworkBehaviour(networkManager)
            {
            }

            void onAttach(ecs::Entity entity) override
            {
                addResponse(
                    {
                        {common::NetworkMessage::serverUpdateShipPosition,
                        [this](rtype::net::Message<common::NetworkMessage> msg)
                        {
                            onUpdateVelocity(msg);
                        }},
                        {common::NetworkMessage::serverAllyTakeDamage,
                        [this](rtype::net::Message<common::NetworkMessage> msg)
                        {
                            onDamageReceive(msg);
                        }},
                        {common::NetworkMessage::serverAllyDestroy,
                        [this](rtype::net::Message<common::NetworkMessage> msg)
                        {
                            onDestroy(msg);
                        }},
                    }
                );
            }

            void onUpdateVelocity(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "onUpdateVelocity" << std::endl;
                common::game::netbody::ServerUpdateShipPosition body;
                auto &allyBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                allyBody.velocity = body.pos;
            }

            void onDamageReceive(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "onDamageReceive" << std::endl;
                auto &allyHealth = _coord->getComponent<ecs::components::health::health_t>(_entity);
                common::game::netbody::ServerAllyTakeDamage body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                allyHealth.healthPoints -= body.damage;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::cout << "onDestroy" << std::endl;
                common::game::netbody::ServerAllyDestroy body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                engine::destroyEntity(_entity);
                unregisterResponses();
            }

            void update() override
            {
            }
    };

}
