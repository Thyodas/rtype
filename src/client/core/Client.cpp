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

#include "client/menu/SkinSelectorBehave.hpp"

#include "raylib.h"

client::Client::Client()
{
    engine::initEngine();
    SetTraceLogLevel(TraceLogLevel::LOG_ERROR);
}

void client::Client::menu()
{
    ecs::Entity selector = engine::createEntity();
    auto selectorBehavior = engine::createBehavior<client::SkinSelectorBehave>();
    engine::attachBehavior(selector, selectorBehavior);

    while (engine::isWindowOpen()) {
        if (selectorBehavior->isSkinSelected()) {
            _playerSkin = selectorBehavior->getSelectedSkin();
            selectorBehavior->onExit();
            engine::destroyEntity(selector);
            break;
        }
        _netClient.dispatchAllResponses();
        engine::runEngine();
    }
}

void client::Client::run()
{
    menu();
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
    engine::playMusic("./ressources/audio/BackgroundMusic.mp3");

    _netClient.connect("127.0.0.1", 5454);

    _netClient.reqPingServer();
    _netClient.reqClientConnect("Jean-Baptiste", _playerSkin);
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
