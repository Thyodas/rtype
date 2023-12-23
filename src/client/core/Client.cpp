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
#include "client/entities/Skybox/SkyboxBehavior.hpp"

client::Client::Client()
{
    engine::initEngine();
}

void client::Client::run()
{
    common::game::EntityFactory factory;
    ecs::Entity cube = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::Transtellar, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 180, 0},
        {1, 1, 1}
    });
    auto spaceShipNetwork = engine::createBehavior<client::PlayerNetwork>(_netClient);
    engine::attachBehavior(cube, spaceShipNetwork);


    auto move = engine::createBehavior<BulletNetwork>(_netClient);
    ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {0.025, 0.025, 0.025}
    }, common::game::ObjectFormat::GLB);
    engine::attachBehavior(gunBullet, move);

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

    // _netClient.connect("localhost", 60000);

    // _netClient.reqPingServer();
    // _netClient.reqClientConnect("Jean-Baptiste", common::game::ObjectName::DualStriker);
    // _netClient.reqPingServer();

    while (engine::isWindowOpen()) {
        // if (!_netClient.isConnected())
            // break;
        // _netClient.dispatchAllResponses();
        // _netClient.reqPingServer();
        engine::runEngine();
    }
    // _netClient.disconnect();
}

client::Client::~Client()
{
}
