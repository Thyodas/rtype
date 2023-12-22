/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#include "client/core/Client.hpp"

#include "client/entities/EntityFactory.hpp"

#include "../MovementBehaviour.hpp"
#include "../TestBehaviour.hpp"

#include "client/entities/Spaceship/SpaceshipNetwork.hpp"

client::Client::Client() : _clock(new Chrono())
{
    engine::initEngine();
}

void client::Client::run()
{
    client::EntityFactory factory;
    ecs::Entity cube = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::RedFighter, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {1, 1, 1}
    });
    auto spaceShipNetwork = engine::createBehavior<client::SpaceshipNetwork>(_netClient);
    engine::attachBehavior(cube, spaceShipNetwork);


    auto move = engine::createBehavior<movement>();
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
    engine::attachBehavior(gunBullet, move);






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
        //_netClient.reqPingServer();
        engine::runEngine();
    }
    _netClient.disconnect();
}

client::Client::~Client()
{
    delete _clock;
}
