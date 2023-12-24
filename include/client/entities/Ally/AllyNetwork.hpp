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
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverUpdateShipPosition, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onUpdatePosition(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverAllyTakeDamage, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDamageReceive(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverAllyDestroy, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDestroy(msg);
                    }},
                });
                _networkManager.registerResponse({
                    {common::NetworkMessage::serverAllyDisconnect, [this](rtype::net::Message<common::NetworkMessage> msg) {
                        onDestroy(msg);
                    }},
                });
            }

            void onUpdatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipPosition body;
                auto &allyBody = _coord->getComponent<ecs::components::physics::rigidBody_t>(_entity);
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                allyBody.velocity = body.pos;
            }

            void onDamageReceive(rtype::net::Message<common::NetworkMessage>& msg)
            {
                auto &allyHealth = _coord->getComponent<ecs::components::health::health_t>(_entity);
                common::game::netbody::ServerAllyTakeDamage body;
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                allyHealth.healthPoints -= body.damage;
            }

            void onDestroy(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerAllyDestroy body;
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
