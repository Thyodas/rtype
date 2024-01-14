/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#include "client/core/Client.hpp"

#include "common/game/entities/EntityFactory.hpp"

#include "client/entities/Player/PlayerNetwork.hpp"
#include "client/entities/Bullet/BulletNetwork.hpp"
#include "client/entities/Skybox/SkyboxBehavior.hpp"

client::Client::Client()
{
    engine::initEngine();
}

void client::Client::run()
{
    common::game::EntityFactory factory;

    auto skyBehavior = engine::createBehavior<client::SkyboxBehavior>();
    ecs::Entity skyBox = factory.createEntity(common::game::ObjectType::SkyBox, common::game::ObjectName::DefaultSkybox, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {1, 1, 1}
    }, common::game::ObjectFormat::PNG);
    engine::attachBehavior(skyBox, skyBehavior);

    _netClient.connect("127.0.0.1", 60000);

    _netClient.reqPingServer();
    _netClient.reqClientConnect("Jean-Baptiste", common::game::ObjectName::DualStriker);
    _netClient.reqPingServer();

    while (engine::isWindowOpen()) {
        if (!_netClient.isConnected())
            break;
        _netClient.dispatchAllResponses();
        _netClient.reqPingServer();
        engine::runEngine();
    }
    _netClient.disconnect();
}

client::Client::~Client()
{
}
