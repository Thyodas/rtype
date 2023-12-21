/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#include "client/core/Client.hpp"

client::Client::Client() : _clock(new Chrono())
{
    engine::initEngine();
}

void client::Client::run()
{
    _netClient.connect("localhost", 60000);
    while (engine::isWindowOpen()) {
        if (!_netClient.isConnected())
            break;
        if (!_netClient.getIncoming().empty()) {
            auto msg = _netClient.getIncoming().pop_front().msg;

            switch (msg.header.id) {
                case CustomMsgTypes::ServerAccept: {
                    // Server has responded to a ping request
                    std::cout << "Server Accepted Connection\n";
                }
                break;

                case CustomMsgTypes::ServerPing: {
                    // Server has responded to a ping request
                    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    msg >> timeThen;
                    std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
                }
                break;

                case CustomMsgTypes::ServerMessage: {
                    // Server has responded to a ping request
                    uint32_t clientID;
                    msg >> clientID;
                    std::cout << "Hello from [" << clientID << "]\n";
                }
                break;
            }
        }
        _netClient.pingServer();
        engine::runEngine();
    }
    _netClient.disconnect();
}

client::Client::~Client()
{
    delete _clock;
}
