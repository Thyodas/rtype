/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** main.cpp
*/

#include "raylib.h"
#include "core/window/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "core/GameEngine.hpp"
#include "TestBehaviour.hpp"

int main()
{
    engine::initEngine();
    ecs::Entity cube = engine::createCube({0, 1, 0}, 2, 2, 2);
    auto behave = std::make_shared<input>();
    engine::attachBehavior(cube, behave);
    ecs::Entity cube2 = engine::createCube({5, 1, 0}, 2, 2, 2);

    while (engine::isWindowOpen())
        engine::runEngine();
    return 0;
}