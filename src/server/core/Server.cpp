/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Server.cpp
*/

#include "server/core/Server.hpp"
//
#include "common/game/entities/EntityFactory.hpp"
#include "game_engine/ecs/components/EnemySpawner.hpp"

server::Server::Server()
    : _netServer(5454)
{

}

void server::Server::run()
{
    _netServer.start();
    engine::initEngine();

    ecs::SceneID sceneID = engine::createScene();
    _netServer.setMainSceneID(sceneID);
    engine::activateScene(sceneID);
    engine::core::EngineCamera camera = engine::createCamera({0, 0, 0}, {0, 0, 0}, {0, 1, 0}, CAMERA_PERSPECTIVE, 90.0f);

    common::game::EntityFactory factory;
    ecs::Entity spawner = engine::createEntity();
    auto behave = engine::createBehavior<ecs::components::behaviour::EnemySpawner>(_netServer, spawner);
    engine::attachBehavior(spawner, behave);
    engine::addEntityToScene(spawner, sceneID);

    while (true) {
        _netServer.update(-1, true);
        engine::update(sceneID);
        //engine::render(sceneID, camera.getCameraID());
    }
}
