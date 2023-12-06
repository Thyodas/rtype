/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** main.cpp
*/

#include "raylib.h"
#include "core/window/Window.hpp"
#include "core/event/Event.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "core/GameEngine.hpp"
#include "TestBehaviour.hpp"

int main()
{
    engine::core::Window window;
    window.setFPS(60);
    engine::initEngine();
    ecs::Entity cube = engine::createCube({0, 1, 0}, 2, 2, 2);
    auto behave = std::make_shared<input>();
    engine::attachBehavior(cube, behave);

    while (window.isOpen()) {
        BeginDrawing();
        window.clear(BLACK);
        BeginMode3D(window.getCamera());
        engine::runEngine();
        DrawGrid(20, 1.0f);
        EndMode3D();
        EndDrawing();
    }
    return 0;
}