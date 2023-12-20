/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Main
*/

#include "client/core/Client.hpp"
#include "client/entities/EntityFactory.hpp"

#include "MovementBehaviour.hpp"
#include "TestBehaviour.hpp"

int main(int ac, char **av)
{
    client::Client client;
    client::EntityFactory factory;
    ecs::Entity cube = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::RedFighter, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {1, 1, 1}
    });
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);


    auto move = engine::createBehavior<movement>();
    ecs::Entity gunBullet = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::GunBullet, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {0.025, 0.025, 0.025}
    }, client::ObjectFormat::GLB);
    engine::attachBehavior(gunBullet, move);

    client.run();

    return 0;
}
