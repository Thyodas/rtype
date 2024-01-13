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

    common::game::EntityFactory factory;
    ecs::Entity spawner = engine::createEntity();
    auto behave = engine::createBehavior<ecs::components::behaviour::EnemySpawner>(_netServer, spawner);
    engine::attachBehavior(spawner, behave);
    while (true) {
        _netServer.update(-1, true);
        engine::runEngine();
    }
}
