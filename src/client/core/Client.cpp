/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#include "client/core/Client.hpp"

client::Client::Client()
{
    engine::initEngine();
}

void client::Client::run()
{
    while (engine::isWindowOpen())
    {
        engine::runEngine();
    }
}

client::Client::~Client()
{
}
