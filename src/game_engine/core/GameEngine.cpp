/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "GameEngine.hpp"

namespace engine {
    void Engine::init()
    {
        _coordinator.init();

        _coordinator.registerComponent<ecs::components::physics::transform_t>();
        _coordinator.registerComponent<ecs::components::render::render_t>();
        _coordinator.registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator.registerComponent<ecs::components::physics::collider_t>();

        ecs::Signature signaturePhysics;
        signaturePhysics.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        ecs::Signature signatureRender;
        signatureRender.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signatureRender.set(_coordinator.getComponentType<ecs::components::render::render_t>());
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signatureBehaviour.set(_coordinator.getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        ecs::Signature signatureCollider;
        signaturePhysics.set(_coordinator.getComponentType<ecs::components::physics::transform_t>());
        signaturePhysics.set(_coordinator.getComponentType<ecs::components::physics::collider_t>());

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
        _collisionDetectionSystem->detectCollision(_coordinator);
        //_physicSystem->updatePosition(_coordinator);
        _renderSystem->render(_coordinator);
    }
}