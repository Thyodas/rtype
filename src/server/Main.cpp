/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.c
*/

#include <iostream>
#include "common/network/network.hpp"

enum class CustomMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomServer : public rtype::net::ServerInterface<CustomMsgTypes> {
    public:
        CustomServer(uint16_t nPort) : rtype::net::ServerInterface<CustomMsgTypes>(nPort)
        {
        }

    protected:
        bool onClientConnect(std::shared_ptr<rtype::net::Connection<CustomMsgTypes>> client) override
        {
            rtype::net::Message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::ServerAccept;
            client->send(msg);
            return true;
        }

        // Called when a client appears to have disconnected
        void onClientDisconnect(std::shared_ptr<rtype::net::Connection<CustomMsgTypes>> client) override
        {
            std::cout << "Removing client [" << client->getID() << "]\n";
        }

        // Called when a message arrives
        void onMessage(std::shared_ptr<rtype::net::Connection<CustomMsgTypes>> client,
            rtype::net::Message<CustomMsgTypes>& msg) override
        {
            switch (msg.header.id) {
                case CustomMsgTypes::ServerPing: {
                    std::cout << "[" << client->getID() << "]: Server Ping\n";

                    // Simply bounce message back to client
                    client->send(msg);
                }
                break;

                case CustomMsgTypes::MessageAll: {
                    std::cout << "[" << client->getID() << "]: Message All\n";

                    // Construct a new message and send it to all clients
                    rtype::net::Message<CustomMsgTypes> msg;
                    msg.header.id = CustomMsgTypes::ServerMessage;
                    msg << client->getID();
                    messageAllClients(msg, client);
                }
                break;
            }
        }
};

int main()
{
    CustomServer server(60000);
    server.start();

    while (true) {
        server.update(-1, true);
    }

    return 0;
}
