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
    engine::Engine engine;
    engine.init();
    ecs::Entity test = engine.addEntity();
    ecs::Entity cube2 = engine.addEntity({4, 1, 0});
    auto behave = std::make_shared<input>();
    engine.addComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(test, behave);
    engine.addComponent<ecs::components::physics::collider_t>(test, {ecs::components::ShapeType::CUBE, ecs::components::physics::CollisionType::COLLIDE, std::make_shared<ecs::components::Cube>()});
    engine.addComponent<ecs::components::physics::collider_t>(cube2, {ecs::components::ShapeType::CUBE, ecs::components::physics::CollisionType::COLLIDE, std::make_shared<ecs::components::Cube>()});

    while (window.isOpen()) {
        BeginDrawing();
        window.clear(BLACK);
        BeginMode3D(window.getCamera());
        engine.run();
        DrawGrid(20, 1.0f);
        EndMode3D();
        EndDrawing();
    }
    return 0;
}