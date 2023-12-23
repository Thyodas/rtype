/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetClient.hpp
*/

#pragma once

#include "common/network/network.hpp"

#include "common/game/NetworkMessage.hpp"
#include "common/game/entities/EntityFactory.hpp"
#include "common/game/NetworkBody.hpp"

#include "common/utils/Chrono.hpp"
#include <chrono>


namespace client {

    class NetClient : public rtype::net::ClientInterface<common::NetworkMessage>
    {
        
        public:

        NetClient()
        {
            registerResponse({
                {common::NetworkMessage::ServerPing, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resPingServer(msg);
                }},
                {common::NetworkMessage::serverFireBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resServerFireBullet(msg);
                }},
                {common::NetworkMessage::serverCreatePlayerShip, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resServerCreatePlayerShip(msg);
                }},
                {common::NetworkMessage::serverAllyConnect, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resServerAllyConnect(msg);
                }},
                {common::NetworkMessage::serverCreateEnemy, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resServerCreateEnemy(msg);
                }},
            });
        }

        void resPingServer(rtype::net::Message<common::NetworkMessage>& msg)
        {
            common::game::netbody::PingServer body;
            msg >> body;

            std::cout << "Ping duration: " << engine::Engine::getInstance()->getElapsedTime() - body.timeStart << std::endl;

        }

        void reqPingServer()
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::ServerPing;

            common::game::netbody::PingServer body = {
                engine::Engine::getInstance()->getElapsedTime()
            };

            msg << body;
            send(msg);
        }

        void reqClientConnect(std::string name, common::game::ObjectName shipName)
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::clientConnect;

            common::game::netbody::ClientConnect body = {
                .name = std::move(name),
                .shipName = shipName
            };

            msg << body;
            send(msg);
        }

        void resServerFireBullet(rtype::net::Message<common::NetworkMessage>& msg);
        void resServerCreatePlayerShip(rtype::net::Message<common::NetworkMessage>& msg);
        void resServerAllyConnect(rtype::net::Message<common::NetworkMessage>& msg);
        void resServerCreateEnemy(rtype::net::Message<common::NetworkMessage>& msg);

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