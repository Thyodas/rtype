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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost::multi_index;

namespace client {

    class NetClient : public rtype::net::ClientInterface<common::NetworkMessage>
    {
        public:

        NetClient() : _nextId(1)
        {
            registerResponse({
                {common::NetworkMessage::ServerPing, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resPingServer(msg);
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
                {common::NetworkMessage::serverFireBullet, [this](rtype::net::Message<common::NetworkMessage> msg) {
                    resServerFireBullet(msg);
                }},
            });
        }

        void resServerFireBullet(rtype::net::Message<common::NetworkMessage>& msg);

        void resPingServer(rtype::net::Message<common::NetworkMessage>& msg)
        {
            common::game::netbody::PingServer body;
            msg >> body;
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

        void resServerCreatePlayerShip(rtype::net::Message<common::NetworkMessage>& msg);
        void resServerAllyConnect(rtype::net::Message<common::NetworkMessage>& msg);
        void resServerCreateEnemy(rtype::net::Message<common::NetworkMessage>& msg);

        void messageAll()
        {
            rtype::net::Message<common::NetworkMessage> msg;
            msg.header.id = common::NetworkMessage::MessageAll;
            send(msg);
        }

        using ResponseFunction = std::function<void(rtype::net::Message<common::NetworkMessage> msg)>;

        struct ResponseRecord {
                int id;
                common::NetworkMessage messageType;
                ResponseFunction func;

                ResponseRecord(int id, common::NetworkMessage messageType, ResponseFunction func)
                    : id(id), messageType(messageType), func(std::move(func)) {}
        };

        struct id {};
        struct message_type {};

        typedef multi_index_container<
            ResponseRecord,
            indexed_by<
                ordered_unique<tag<id>, member<ResponseRecord, int, &ResponseRecord::id>>,
                ordered_non_unique<tag<message_type>, member<ResponseRecord, common::NetworkMessage, &ResponseRecord::messageType>>
            >
        > ResponseSet;

        int registerResponse(common::NetworkMessage messageType, const ResponseFunction& func)
        {
            int id = _nextId++;
            _responses.insert(ResponseRecord(id, messageType, func));
            return id;
        }

        std::vector<int> registerResponse(const std::vector<std::pair<common::NetworkMessage, ResponseFunction>>& responses)
        {
            std::vector<int> registeredIds;
            for (const auto& response : responses) {
                int id = _nextId++;
                _responses.insert(ResponseRecord(id, response.first, response.second));
                registeredIds.push_back(id);
            }
            return registeredIds;
        }

        void unregisterResponse(int responseId)
        {
            auto& id_index = _responses.get<id>();
            id_index.erase(responseId);
        }

        void dispatchResponse(rtype::net::Message<common::NetworkMessage>& msg)
        {
            std::vector<ResponseFunction> functionsToCall;

            auto& message_type_index = _responses.get<message_type>();
            auto range = message_type_index.equal_range(msg.header.id);

            for (auto it = range.first; it != range.second; ++it) {
                functionsToCall.push_back(it->func);
            }

            for (auto& func : functionsToCall) {
                func(msg);
            }
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
            ResponseSet _responses;
            int _nextId;
    };
}