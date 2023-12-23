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

namespace server {
    class NetServer : public rtype::net::ServerInterface<common::NetworkMessage> {
        public:
            NetServer(uint16_t nPort) : rtype::net::ServerInterface<common::NetworkMessage>(nPort)
            {
            }

        protected:
            bool onClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client) override
            {
                rtype::net::Message<common::NetworkMessage> msg;
                msg.header.id = common::NetworkMessage::ServerAccept;
                client->send(msg);
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
                switch (msg.header.id) {
                    case common::NetworkMessage::ServerPing: {
                        std::cout << "[" << client->getID() << "]: Server Ping\n";

                        // Simply bounce message back to client
                        client->send(msg);
                    }
                    break;

                    case common::NetworkMessage::MessageAll: {
                        std::cout << "[" << client->getID() << "]: Message All\n";

                        // Construct a new message and send it to all clients
                        rtype::net::Message<common::NetworkMessage> msg;
                        msg.header.id = common::NetworkMessage::ServerMessage;
                        msg << client->getID();
                        messageAllClients(msg, client);
                    }
                    break;
                    case common::NetworkMessage::clientPlayerFireBullet: {
                        std::cout << "[" << client->getID() << "]: Player Fire Bullet\n";

                        // Construct a new message and send it to all clients
                        rtype::net::Message<common::NetworkMessage> msg;
                        msg.header.id = common::NetworkMessage::serverFireBullet;
                        msg << client->getID();
                        messageAllClients(msg);
                    }
                    break;
                }
            }

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
            std::unordered_multimap<common::NetworkMessage, ResponseFunction> _responses;
    };
}
