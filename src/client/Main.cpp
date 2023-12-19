/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Main
*/

#include "client/core/Client.hpp"

int main(int ac, char **av)
{
    client::Client client;
    ecs::Entity cube = engine::createCube({0, 1, 0}, 4, 4, 4, RED, true);

    client.run();
    return 0;
}
