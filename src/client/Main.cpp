/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Main
*/

#include "client/core/Client.hpp"
#include "client/entities/EntityFactory.hpp"

int main(int ac, char **av)
{
    client::Client client;
    client::EntityFactory factory;
    // ecs::Entity cube = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::RedFighter, {
    //     {0, 0, 0},
    //     0,
    //     0,
    //     0,
    //     RED,
    //     false,
    //     NULL
    // });
    ecs::Entity cube = factory.createEntity(client::ObjectType::Cube, client::ObjectName::None, {
        {0, 1, 0},
        4,
        4,
        4,
        RED,
        true,
        RED
    });
    // ecs::Entity cube2 = engine::createCube({0, 1, 0}, 4, 4, 4, RED, true);

    client.run();
    return 0;
}
