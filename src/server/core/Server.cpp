/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Server.cpp
*/

#include "server/core/Server.hpp"

server::Server::Server()
    : _netServer(60000)
{

}

void server::Server::run()
{
    _netServer.start();

    engine::initEngine(true);

    while (true) {
        _netServer.update(-1, true);
        engine::runEngine();
    }
}
