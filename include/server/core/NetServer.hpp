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

namespace server {
    class NetServer : public rtype::net::ServerInterface<common::NetworkMessage> {
        public:
            NetServer(uint16_t nPort) : rtype::net::ServerInterface<common::NetworkMessage>(nPort)
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



            void resPingServer(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, const rtype::net::Message<common::NetworkMessage>& msg);
            void resClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg);


            void reqServerCreatePlayerShip(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, ecs::Entity ship);

            void allServerAllyConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, ecs::Entity ship);
            void allServerUpdateShipPosition(ecs::Entity ship);

            using ResponseFunction = std::function<void(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client, rtype::net::Message<common::NetworkMessage> msg)>;


            void registerResponse(common::NetworkMessage id, const ResponseFunction& func)
            {
                _responses.insert(std::make_pair(id, func));
            }

            void registerResponse(const std::vector<std::pair<common::NetworkMessage, ResponseFunction>>& responses)
            {
                for (auto &response : responses)
                    _responses.insert(response);
            }

            void dispatchResponse(const std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, const rtype::net::Message<common::NetworkMessage>& msg)
            {
                const auto &[first, second] = _responses.equal_range(msg.header.id);
                for (auto it = first; it != second; ++it)
                    it->second(client, msg);
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
            std::unordered_multimap<common::NetworkMessage, ResponseFunction> _responses;
    };
}
