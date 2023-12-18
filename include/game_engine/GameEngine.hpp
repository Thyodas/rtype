/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.hpp
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "core/Window.hpp"
#include "./ecs/systems/Physics.hpp"
#include "game_engine/ecs/systems/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Entity.hpp"
#include "game_engine/ecs/components/Animations.hpp"
#include "game_engine/ecs/systems/Animations.hpp"
#include <memory>
#include <mutex>
#include <functional>
#include <utility>

namespace engine {
    class Engine {
        public:
            void init();

            ecs::Entity addEntity(ecs::components::physics::transform_t transf = {{0, 1, 0}, {0}, {0}},
                                    ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, std::make_shared<ecs::components::Cube>()}
            );

            template<typename T>
            void addComponent(ecs::Entity entity, T component) {
                _coordinator->addComponent<T>(entity, component);
            }

            template<typename T>
            T &getComponent(ecs::Entity entity) {
                return _coordinator->getComponent<T>(entity);
            }

            template<typename T>
            void registerListener(std::function<void(const T&)> listener)
            {
                _coordinator->registerListener<T>(listener);
            }

            bool isWindowOpen(void)
            {
                return _window.isOpen();
            }

            void run();

        private:
            std::shared_ptr<ecs::Coordinator> _coordinator;
            std::shared_ptr<ecs::system::PhysicsSystem> _physicSystem;
            std::shared_ptr<ecs::system::RenderSystem> _renderSystem;
            std::shared_ptr<ecs::system::BehaviourSystem> _behaviourSystem;
            std::shared_ptr<ecs::system::AnimationSystem> _animationSystem;
            std::shared_ptr<ecs::system::CollisionResponse> _collisionResponseSystem;
            std::shared_ptr<ecs::system::ColisionDetectionSystem> _collisionDetectionSystem;

            core::Window _window{};

        private:
            static Engine *engine;
            static std::mutex _mutex;

        protected:
            Engine() {};
            ~Engine() {};

        public:
            Engine(Engine &other) = delete;
            void operator=(const Engine &) = delete;

            static Engine *getInstance() {
                std::lock_guard<std::mutex> lock(_mutex);
                if (engine == nullptr)
                    engine = new Engine;
                return engine;
            };
    };

    void initEngine();
    void runEngine();
    ecs::Entity createCube(Vector3 pos, float width, float height, float length, Color color = RED, bool toggleWire = false, Color wireColor = BLACK);
    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color = WHITE);
    template<typename T, typename... Args>
    std::shared_ptr<T> createBehavior(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    void attachBehavior(ecs::Entity entity, std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour);
    bool isWindowOpen(void);
    void setRotation(ecs::Entity entity, Vector3 rotation);
    void setAnimation(ecs::Entity entity, const char *filename);

    template<typename T>
    void registerListener(std::function<void(const T&)> listener)
    {
        Engine::getInstance()->registerListener<T>(listener);
    }
}