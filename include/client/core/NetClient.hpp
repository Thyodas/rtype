/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetClient.hpp
*/

#pragma once

#include "common/network/network.hpp"

#include "common/game/NetworkMessage.hpp"
#include "client/entities/EntityFactory.hpp"
#include "common/game/NetworkBody.hpp"

namespace client {

    class NetClient : public rtype::net::ClientInterface<common::NetworkMessage>
    {
        public:
        void reqPingServer()
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::ServerPing;

            std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

            msg << timeNow;
            send(msg);
        }

        void reqClientConnect(std::string name, client::ObjectName shipName)
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::clientConnect;

            common::game::netbody::ClientConnect body = {
                .name = name,
                .shipName = shipName
            };

            msg << body;
            send(msg);
        }

        void resServerFireBullet(rtype::net::Message<common::NetworkMessage>& msg)
        {
            common::game::netbody::ServerFireBullet body;

            msg >> body;

            client::EntityFactory factory;
            ecs::Entity gunBullet = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::GunBullet, {
                {0, 0, 0},
                0,
                0,
                0,
                WHITE,
                false,
                WHITE,
                {0, 0, 0},
                {0.025, 0.025, 0.025}
            }, client::ObjectFormat::GLB);
            // engine::attachBehavior(gunBullet, BulletNetwork);
        }

        void messageAll()
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::MessageAll;
            send(msg);
        }

        void registerResponse(common::NetworkMessage id, const std::function<void(rtype::net::Message<common::NetworkMessage>)> func)
        {
            _responses.insert(std::make_pair(id, func));
        }

        void registerResponse(std::vector<std::pair<common::NetworkMessage, std::function<void(rtype::net::Message<common::NetworkMessage>)>>> responses)
        {
            for (auto &response : responses)
                _responses.insert(response);
        }

        void dispatchResponse(rtype::net::Message<common::NetworkMessage>& msg)
        {
            const auto &[first, second] = _responses.equal_range(msg.header.id);
            for (auto it = first; it != second; ++it)
                it->second(msg);
        }

        void dispatchAllResponses()
        {
            auto &incoming = getIncoming();
            while (!incoming.empty()) {
                auto msg = incoming.pop_front().msg;
                dispatchResponse(msg);
            }
        }

        protected:
            std::unordered_multimap<common::NetworkMessage, std::function<void(rtype::net::Message<common::NetworkMessage>)>> _responses;
    };
}