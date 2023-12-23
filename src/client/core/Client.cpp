/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#include "client/core/Client.hpp"

#include "common/game/entities/EntityFactory.hpp"

#include "../MovementBehaviour.hpp"
#include "../TestBehaviour.hpp"

#include "client/entities/Player/PlayerNetwork.hpp"

#include "client/entities/Bullet/BulletNetwork.hpp"

client::Client::Client()
{
    engine::initEngine();
}

void client::Client::run()
{
    _netClient.connect("localhost", 60000);

    _netClient.reqPingServer();
    _netClient.reqClientConnect("Jean-Baptiste", common::game::ObjectName::DualStriker);
    _netClient.reqPingServer();

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
}
