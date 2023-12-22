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

#include "client/entities/Player/PlayerNetwork.hpp"

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
    auto spaceShipNetwork = engine::createBehavior<client::PlayerNetwork>(_netClient);
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

    _netClient.reqClientConnect("Jean-Baptiste", client::ObjectName::DualStriker);

    while (engine::isWindowOpen()) {
        if (!_netClient.isConnected())
            break;
        _netClient.dispatchAllResponses();
        //_netClient.reqPingServer();
        engine::runEngine();
    }
    _netClient.disconnect();
}

client::Client::~Client()
{
    delete _clock;
}
