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
#include "common/utils/Math.hpp"

namespace client {

    class AllyNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient> {
        public:
            explicit AllyNetwork(client::NetClient& networkManager, uint32_t netId = 0, ecs::SceneID sceneId = 0)
                : NetworkBehaviour(networkManager, netId, 0, sceneId)
            {
            }

            void onAttach(ecs::Entity entity) override
            {
                addResponse({
                    {common::NetworkMessage::serverUpdateShipPosition,
                    [this](rtype::net::Message<common::NetworkMessage> msg)
                    {
                        onUpdatePosition(msg);
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
                });
            }

            void onUpdatePosition(rtype::net::Message<common::NetworkMessage>& msg)
            {
                common::game::netbody::ServerUpdateShipPosition body;
                auto &allyBody = _coord->getComponent<ecs::components::physics::TransformComponent>(_entity);
                msg >> body;

                if (body.entityNetId != getNetId())
                    return;

                allyBody.position = common::utils::rayVectorToJoltVector(body.pos);
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

                engine::destroyEntity(_entity);
            }

            void update() override
            {
            }
    };

}
