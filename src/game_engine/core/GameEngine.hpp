/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.hpp
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/systems/Physics.hpp"
#include "game_engine/ecs/systems/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Entity.hpp"
#include <memory>

namespace engine {
    class Engine {
        public:
            Engine() {};
            ~Engine() {};

            void init();

            ecs::Entity addEntity(ecs::components::physics::transform_t transf = {{0, 1, 0}, {0}, {0}},
                                    ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, std::make_shared<ecs::components::Cube>()}
            );

            template<typename T>
            void addComponent(ecs::Entity entity, T component) {
                _coordinator.addComponent<T>(entity, component);
            }

            void run();

        private:
            ecs::Coordinator _coordinator;
            std::shared_ptr<ecs::system::PhysicsSystem> _physicSystem;
            std::shared_ptr<ecs::system::RenderSystem> _renderSystem;
            std::shared_ptr<ecs::system::BehaviourSystem> _behaviourSystem;
            std::shared_ptr<ecs::system::ColisionDetectionSystem> _collisionDetectionSystem;
    };
}