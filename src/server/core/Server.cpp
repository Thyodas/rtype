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
    : _netServer(60000)
{

}

void server::Server::run()
{
    _netServer.start();
    engine::initEngine();

    // créer un cube et attacher le enemyspawn behavior
    common::game::EntityFactory factory;
    ecs::Entity cube = factory.createEntity(common::game::ObjectType::Cube, common::game::ObjectName::None, {
            {0, 0, 0},
            0,
            0,
            0,
            RED,
            false,
            RED,
            {0, 0, 0},
            {50, 50, 50}
    }, common::game::ObjectFormat::F_NONE);
    auto behave = engine::createBehavior<ecs::components::behaviour::EnemySpawner>(_netServer, cube);
    engine::attachBehavior(cube, behave);
    while (true) {
        _netServer.update(-1, true);
        engine::runEngine();
    }
}
