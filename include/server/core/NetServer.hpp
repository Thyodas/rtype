/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetClient.hpp
*/

#pragma once

#include <iostream>

#include "common/network/network.hpp"

#include "game_engine/GameEngine.hpp"

#include "common/game/NetworkMessage.hpp"
#include "common/game/NetworkBody.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost::multi_index;

namespace server {
    class NetServer : public rtype::net::ServerInterface<common::NetworkMessage> {
        public:
            NetServer(uint16_t nPort) : rtype::net::ServerInterface<common::NetworkMessage>(nPort), _nextId(1)
            {
                registerResponse({
                    {
                        common::NetworkMessage::ServerPing,
                        [this](std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg) {
                           resPingServer(client, msg);
                       }
                    },
                    {
                        common::NetworkMessage::clientConnect,
                        [this](std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg) {
                            resClientConnect(client, msg);
                        }
                    },
                });
            }

            void setMainSceneID(ecs::SceneID mainSceneID)
            {
                _mainSceneID = mainSceneID;
            }



            void resPingServer(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, const rtype::net::Message<common::NetworkMessage>& msg);
            void resClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg);


            void reqServerCreatePlayerShip(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, ecs::Entity ship);

            void allServerAllyConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, ecs::Entity ship);
            void allServerUpdateShipPosition(ecs::Entity ship);
            void allServerFireBullet(ecs::Entity bullet, ecs::Entity sender);

            using ResponseFunction = std::function<void(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg)>;

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

            void dispatchResponse(const std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, const rtype::net::Message<common::NetworkMessage>& msg)
            {
                std::vector<ResponseFunction> functionsToCall;

                auto& message_type_index = _responses.get<message_type>();
                auto range = message_type_index.equal_range(msg.header.id);

                for (auto it = range.first; it != range.second; ++it) {
                    functionsToCall.push_back(it->func);
                }

                for (auto& func : functionsToCall) {
                    func(client, msg);
                }
            }


        protected:
            bool onClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client) override
            {
                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::ServerAccept;
                messageClient(client, msg);
                return true;
            }

            // Called when a client appears to have disconnected
            void onClientDisconnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client) override
            {
                std::cout << "Removing client [" << client->getID() << "]\n";
            }

            // Called when a message arrives
            void onMessage(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client,
                rtype::net::Message<common::NetworkMessage>& msg) override
            {
                dispatchResponse(client, msg);
            }

        protected:
            ResponseSet _responses;
            ecs::SceneID _mainSceneID;
            int _nextId;
    };
}
