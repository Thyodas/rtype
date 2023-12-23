/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include "raymath.h"

std::shared_ptr<ecs::Coordinator> ecs::components::behaviour::Behaviour::_coord = nullptr;
std::shared_ptr<ecs::Coordinator> ecs::system::System::_coord = nullptr;
namespace engine {

    Engine *Engine::engine = nullptr;
    std::mutex Engine::_mutex;
    void Engine::init(bool disableRender)
    {
        _disableRender = disableRender;
        if (!_disableRender) {
            _window = std::make_shared<core::Window>();
            _window->setFPS(60);
        }
        _coordinator = std::make_shared<ecs::Coordinator>();
        _coordinator->init();
        _collisionResponseSystem = std::make_shared<ecs::system::CollisionResponse>(*_coordinator);
        ecs::components::behaviour::Behaviour::_coord = _coordinator;
        ecs::system::System::_coord = _coordinator;

        _coordinator->registerComponent<ecs::components::physics::transform_t>();
        if (!_disableRender)
            _coordinator->registerComponent<ecs::components::render::render_t>();
        _coordinator->registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator->registerComponent<ecs::components::physics::collider_t>();
        _coordinator->registerComponent<ecs::components::physics::rigidBody_t>();
        _coordinator->registerComponent<ecs::components::animations::animation_t>();
        _coordinator->registerComponent<ecs::components::network::network_t>();
        _coordinator->registerComponent<ecs::components::health::health_t>();

        ecs::Signature signaturePhysics;
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::rigidBody_t>());
        ecs::Signature signatureRender;
        if (!_disableRender) {
            signatureRender.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
            signatureRender.set(_coordinator->getComponentType<ecs::components::render::render_t>());
        }
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator->getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        ecs::Signature signatureCollider;
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::collider_t>());
        ecs::Signature signatureAnimations;
        signatureAnimations.set(_coordinator->getComponentType<ecs::components::animations::animation_t>());

        _physicSystem = _coordinator->registerSystem<ecs::system::PhysicsSystem>();
        _coordinator->setSystemSignature<ecs::system::PhysicsSystem>(signaturePhysics);

        if (!_disableRender) {
            _renderSystem = _coordinator->registerSystem<ecs::system::RenderSystem>();
            _coordinator->setSystemSignature<ecs::system::RenderSystem>(signatureRender);
        }

        _behaviourSystem = _coordinator->registerSystem<ecs::system::BehaviourSystem>();
        _coordinator->setSystemSignature<ecs::system::BehaviourSystem>(signatureBehaviour);

        _collisionDetectionSystem = _coordinator->registerSystem<ecs::system::ColisionDetectionSystem>();
        _coordinator->setSystemSignature<ecs::system::ColisionDetectionSystem>(signatureCollider);

        _animationSystem = _coordinator->registerSystem<ecs::system::AnimationSystem>();
        _coordinator->setSystemSignature<ecs::system::AnimationSystem>(signatureAnimations);
    }

    ecs::Entity Engine::addEntity(ecs::components::physics::transform_t transf, ecs::components::render::render_t render) {
        ecs::Entity entity = _coordinator->createEntity();
        _coordinator->addComponent<ecs::components::physics::transform_t>(entity, transf);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    void Engine::run(void) {
        _behaviourSystem->handleBehaviours();
        _physicSystem->updatePosition();
        _animationSystem->handleAnimations();
        _collisionDetectionSystem->detectCollision();
        _coordinator->dispatchEvents();
        if (_disableRender)
            return;
        _window->clear(BLACK);
        BeginDrawing();
        BeginMode3D(_window->getCamera());
        _renderSystem->render();
        DrawGrid(20, 1.0f);
        EndMode3D();
        EndDrawing();
    }

    void initEngine(bool disableRender)
    {
        Engine::getInstance()->init(disableRender);
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
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::CUBE,
            ecs::components::physics::CollisionType::COLLIDE,
            cube,
            GetModelBoundingBox(cube->getModel()),
            {0},
            MatrixIdentity(),
            MatrixIdentity(),
            MatrixIdentity()};
        Matrix matTranslate = MatrixTranslate(pos.x, pos.y, pos.z);
        collider.matTranslate = MatrixMultiply(collider.matTranslate, matTranslate);
        ecs::components::health::health_t health = {0};
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        return entity;
    }

    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Model3D>(filename, color);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::components::health::health_t health = {0};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        return entity;
    }

    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Skybox>(filename);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        return entity;
    }

    void setAnimation(ecs::Entity entity, const char *filename)
    {
        ecs::components::animations::animation_t anim;
        auto &draw = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        anim.animFrameCounter = 0;
        anim.model = draw.data->getModel();
        anim.animsCount = 0;
        anim.anims = LoadModelAnimations(filename, (int *)&anim.animsCount);
        Engine::getInstance()->addComponent<ecs::components::animations::animation_t>(entity, anim);
    }

    void rotate(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        transform.rotation = Vector3Add(transform.rotation, rotation);
        Matrix matTemp = MatrixRotateXYZ(rotation);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void scale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        transform.scale = Vector3Add(transform.scale, scale);
        Matrix matTemp = MatrixScale(scale.x, scale.y, scale.z);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matScale = MatrixMultiply(collider.matScale, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void attachBehavior(
        ecs::Entity entity,
        std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour
        )
    {
        Engine::getInstance()->addComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity, behaviour);
        behaviour->setEntity(entity);
    }

    bool isWindowOpen(void)
    {
        return Engine::getInstance()->isWindowOpen();
    }
}
