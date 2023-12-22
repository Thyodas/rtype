/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.c
*/

#include <iostream>
#include "common/network/network.hpp"

#include "game_engine/GameEngine.hpp"

#include "common/game/NetworkMessage.hpp"


class CustomServer : public rtype::net::ServerInterface<common::NetworkMessage> {
    public:
        CustomServer(uint16_t nPort) : rtype::net::ServerInterface<common::NetworkMessage>(nPort)
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
};

int main()
{
    CustomServer server(60000);
    server.start();

    engine::initEngine(true);

    while (true) {
        server.update(-1, true);
        engine::runEngine();
    }

    return 0;
}
