/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "GameEngine.hpp"
#include "raymath.h"

namespace engine {

    Engine *Engine::engine = nullptr;
    std::mutex Engine::_mutex;

    void Engine::init()
    {
        _window.setFPS(60);
        _coordinator.init();

        _coordinator.registerComponent<ecs::components::physics::transform_t>();
        _coordinator.registerComponent<ecs::components::render::render_t>();
        _coordinator.registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator.registerComponent<ecs::components::physics::collider_t>();
        _coordinator.registerComponent<ecs::components::physics::rigidBody_t>();

        ecs::Signature signaturePhysics;
        signaturePhysics.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signaturePhysics.set(_coordinator.getComponentType<ecs::components::physics::rigidBody_t>());
        ecs::Signature signatureRender;
        signatureRender.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signatureRender.set(_coordinator.getComponentType<ecs::components::render::render_t>());
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signatureBehaviour.set(_coordinator.getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        signatureBehaviour.set(_coordinator.getComponentType<ecs::components::physics::rigidBody_t>());
        ecs::Signature signatureCollider;
        signatureCollider.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signatureCollider.set(_coordinator.getComponentType<ecs::components::physics::collider_t>());

        _physicSystem = _coordinator.registerSystem<ecs::system::PhysicsSystem>();
        _coordinator.setSystemSignature<ecs::system::PhysicsSystem>(signaturePhysics);

        _renderSystem = _coordinator.registerSystem<ecs::system::RenderSystem>();
        _coordinator.setSystemSignature<ecs::system::RenderSystem>(signatureRender);

        _behaviourSystem = _coordinator.registerSystem<ecs::system::BehaviourSystem>();
        _coordinator.setSystemSignature<ecs::system::BehaviourSystem>(signatureBehaviour);

        _collisionDetectionSystem = _coordinator.registerSystem<ecs::system::ColisionDetectionSystem>();
        _coordinator.setSystemSignature<ecs::system::ColisionDetectionSystem>(signatureCollider);
    }

    ecs::Entity Engine::addEntity(ecs::components::physics::transform_t transf, ecs::components::render::render_t render) {
        ecs::Entity entity = _coordinator.createEntity();
        _coordinator.addComponent<ecs::components::physics::transform_t>(entity, transf);
        _coordinator.addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    void Engine::run(void) {
        _behaviourSystem->handleBehaviours(_coordinator);
        _physicSystem->updatePosition(_coordinator);
        _collisionDetectionSystem->detectCollision(_coordinator);
        _coordinator.dispatchEvents();
        BeginDrawing();
        _window.clear(BLACK);
        BeginMode3D(_window.getCamera());
        _renderSystem->render(_coordinator);
        DrawGrid(20, 1.0f);
        EndMode3D();
        EndDrawing();
    }

    void initEngine()
    {
        Engine::getInstance()->init();
    }

    void runEngine()
    {
        Engine::getInstance()->run();
    }

    ecs::Entity createCube(
        Vector3 pos,
        float width,
        float height,
        float length,
        Color color,
        bool toggleWire,
        Color wireColor
        )
    {
        auto cube = std::make_shared<ecs::components::Cube>(width, height, length, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, cube};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::CUBE, ecs::components::physics::CollisionType::COLLIDE, cube};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        return entity;
    }

    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Model3D>(filename, color);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        return entity;
    }

    void setRotation(ecs::Entity entity, Vector3 rotation)
    {
        auto transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        transform.rotation = rotation;
        render.data->getModel().transform = MatrixRotateXYZ(rotation);
    }

    void attachBehavior(
        ecs::Entity entity,
        std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour
        )
    {
        Engine::getInstance()->addComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity, behaviour);;
    }

    bool isWindowOpen(void)
    {
        return Engine::getInstance()->isWindowOpen();
    }
}