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
    SetTraceLogLevel(TraceLogLevel::LOG_ERROR);
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

    ecs::Entity spawner = factory.createEntity(
        common::game::ObjectType::Cube,
        common::game::ObjectName::None,
        {
            .pos = {0, 0, 0},
            .width = 1,
            .height = 1,
            .length = 1,
            .color = {255, 0, 0, 255},
            .toggleWire = false,
            .wireColor = {0, 0, 0, 255},
            .rotation = {0, 0, 0},
            .scale = {1, 1, 1},
        },
        common::game::ObjectFormat::F_NONE,
        ""
    );

    auto &collider = engine::Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(spawner);
    collider.collisionType = ecs::components::physics::CollisionType::NON_COLLIDE;

    auto behave = engine::createBehavior<ecs::components::behaviour::EnemySpawner>(_netServer, spawner);
    engine::attachBehavior(spawner, behave);

    engine::addEntityToScene(spawner, sceneID);

    while (true) {
        _netServer.update(-1, true);
        engine::update(sceneID);
        // engine::render(sceneID, camera.getCameraID());
    }
}
