/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** main.cpp
*/

#include "my_raylib.h"
#include "game_engine/core/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/ecs/components/Input.hpp"
#include "game_engine/GameEngine.hpp"
#include "TestBehaviour.hpp"

float deg2rad(float angle)
{
    return angle * M_PI / 180;
}

int main()
{
    engine::initEngine();
    ecs::Entity cube = engine::createCube({0, 2, 0}, 4, 4, 4, RED, true);
    //engine::setRotation(cube, {deg2rad(30), 0, 0});
    //engine::setRotation(cube, {deg2rad(-10), 0, 0});
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);
    ecs::Entity cube2 = engine::createCube({5, 1, 0}, 2, 2, 2);
    // ecs::Entity robot = engine::createModel3D("robot.glb", {0, 1, 0}, WHITE);
    // engine::attachBehavior(robot, behave);
    // engine::setAnimation(robot, "robot.glb");

    while (engine::isWindowOpen()) {
        engine::runEngine();
        if (engine::isKeyPressed(ecs::components::input::Keys::KeyR))
            std::cout << "Key pressed" << std::endl;
    }
    return 0;
}
