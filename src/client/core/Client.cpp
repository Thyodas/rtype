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

void client::Client::menu(ecs::SceneID sceneID, engine::core::EngineCamera& camera)
{
    ecs::Entity selector = engine::createEntity();
    auto selectorBehavior = engine::createBehavior<client::SkinSelectorBehave>(sceneID);
    engine::attachBehavior(selector, selectorBehavior);
    engine::addEntityToScene(selector, sceneID);

    while (engine::isWindowOpen()) {
        if (selectorBehavior->isSkinSelected()) {
            _playerSkin = selectorBehavior->getSelectedSkin();
            selectorBehavior->onExit();
            engine::destroyEntity(selector);
            break;
        }
        _netClient.dispatchAllResponses();
        engine::update(sceneID);
        engine::render(sceneID, camera.getCameraID());
    }
}

void client::Client::run()
{

    common::game::EntityFactory factory;
    ecs::SceneID sceneID = engine::createScene();
    engine::core::EngineCamera camera = engine::createCamera({-25, 1, 0}, {0, 0, 0}, {0, 1, 0}, CAMERA_PERSPECTIVE, 45.0f);
    engine::attachCamera(sceneID, camera);
    engine::activateScene(sceneID);
    _netClient.setMainSceneID(sceneID);

    ecs::Entity musicSource = engine::playMusic("./ressources/audio/BackgroundMusic.mp3");
    engine::addEntityToScene(musicSource, sceneID);

    menu(sceneID, camera);

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
    engine::addEntityToScene(skyBox, sceneID);


    _netClient.connect("127.0.0.1", 5454);

    _netClient.reqPingServer();
    _netClient.reqClientConnect("Jean-Baptiste", _playerSkin);
    _netClient.reqPingServer();


    while (engine::isWindowOpen()) {
        if (!_netClient.isConnected())
            break;
        _netClient.dispatchAllResponses();
        _netClient.reqPingServer();
        engine::update(sceneID);
        engine::render(sceneID, camera.getCameraID());
    }
    _netClient.disconnect();
}

client::Client::~Client()
{
}
