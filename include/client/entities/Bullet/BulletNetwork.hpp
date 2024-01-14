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
#include "common/utils/Math.hpp"


namespace client
{

    class BulletNetwork : public ecs::components::behaviour::NetworkBehaviour<client::NetClient>
    {
    public:
        explicit BulletNetwork(client::NetClient &networkManager, uint32_t netId = 0, ecs::SceneID sceneId = 0)
            : NetworkBehaviour(networkManager, netId, 0, sceneId)
        {
        }

        void onAttach(ecs::Entity entity) override
        {
            addResponse({
                {common::NetworkMessage::serverDestroyBullet,
                [this](rtype::net::Message<common::NetworkMessage> msg)
                {
                    onDestroy(msg);
                }},
                {common::NetworkMessage::serverUpdateBulletPosition,
                [this](rtype::net::Message<common::NetworkMessage> msg)
                {
                    updatePosition(msg);
                }},
            });
        }

        void onDestroy(rtype::net::Message<common::NetworkMessage> &msg)
        {
            common::game::netbody::ServerDestroyBullet body;
            msg >> body;

            if (body.entityNetId != getNetId())
                return;

            engine::destroyEntity(_entity);
            unregisterResponses();
        }

        void updatePosition(rtype::net::Message<common::NetworkMessage> &msg)
        {
            common::game::netbody::ServerUpdateBulletPosition body;
            msg >> body;
            std::cout << "try update bullet " << body.entityNetId << std::endl;
            if (body.entityNetId != getNetId())
                return;
            auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(_entity);
            transform.position = common::utils::rayVectorToJoltVector(body.pos);
        }

        void update() override
        {
        }

    private:
        double _lastUpdate = 0;
    };
}
