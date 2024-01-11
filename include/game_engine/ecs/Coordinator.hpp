/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Coordinator.hpp
*/

#pragma once

#include <memory>

#include "System.hpp"
#include "../core/event/Event.hpp"
#include "SingletonComponent.hpp"

namespace ecs {
    /**
     * @class Coordinator
     *
     * @brief Central orchestrator for the Entity-Component-System (ECS) architecture.
     *
     * The Coordinator class ties together the functionalities of the EntityManager,
     * ComponentManager, and SystemManager to facilitate the creation, management,
     * and interaction of entities, components, and systems within the ECS framework.
     */
    class Coordinator {
        public:
            /**
            * @brief Initializes the Coordinator, creating instances of EntityManager,
            * ComponentManager, and SystemManager.
            */
            void init() {
                _componentManager = std::make_shared<components::ComponentManager>();
                _entityManager = std::make_shared<EntityManager>();
                _systemManager = std::make_shared<system::SystemManager>();
                _eventManager = std::make_shared<ecs::event::EventManager>();
                _singletonComponentManager = std::make_shared<ecs::SingletonComponentManager>();
            }

            /**
            * @brief Creates a new entity.
            *
            * @return Entity - The newly created entity's ID.
            */
            Entity createEntity() {
                return _entityManager->createEntity();
            }

            /**
            * @brief Destroys an entity and cleans up its components and system references.
            *
            * @param entity - The ID of the entity to destroy.
            */
            void destroyEntity(Entity entity) {
                _entityManager->destroyEntity(entity);
                _componentManager->entityDestroyed(entity);
                _systemManager->entityDestroyed(entity);
            }

            /**
            * @brief Registers a new component type within the ComponentManager.
            */
            template <typename T>
            void registerComponent() {
                _componentManager->registerComponent<T>();
            }

            /**
             * @brief Registers a new singleton component
             *
             * @tparam T Class that should inherit from SingletonComponent class
             * @param component
             */
            template <typename T>
            void registerSingletonComponent(T component) {
                _singletonComponentManager->registerSingletonComponent<T>(component);
            }

            /**
            * @brief Adds a component to an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            * @param component - The component to add to the entity.
            */
            template <typename T>
            void addComponent(Entity entity, T component) {
                _componentManager->addComponent<T>(entity, component);

                auto signature = _entityManager->getSignature(entity);
                signature.set(_componentManager->getComponentType<T>(), true);
                _entityManager->setSignature(entity, signature);

                _systemManager->entitySignatureChanged(entity, signature);
            }

            /**
            * @brief Removes a component from an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            */
            template<typename T>
            void removeComponent(Entity entity) {
                _componentManager->removeComponent<T>(entity);

                auto signature = _entityManager->getSignature(entity);
                signature.set(_componentManager->getComponentType<T>(), false);
                _entityManager->setSignature(entity, signature);

                _systemManager->entitySignatureChanged(entity, signature);
            }

            /**
             * @brief Remove a singleton component
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             */
            template <typename T>
            void removeSingletonComponent(void) {
                _singletonComponentManager->unregisterSingletonComponent<T>();
            }

            /**
            * @brief Retrieves a reference to a component of an entity.
            *
            * @param entity - The ID of the entity.
            * @return T& - Reference to the requested component.
            */
            template <typename T>
            T &getComponent(Entity entity) {
                return _componentManager->getComponent<T>(entity);
            }

            /**
             * @brief Get the Singleton Component object
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             * @return T& The instance of the desired singleton component
             */
            template <typename T>
            T &getSingletonComponent(void) {
                return _singletonComponentManager->getSingletonComponent<T>();
            }

            /**
            * @brief Gets the component type ID for a specific component type.
            *
            * @return components::ComponentType - The ID of the component type.
            */
            template<typename T>
            components::ComponentType getComponentType() {
                return _componentManager->getComponentType<T>();
            }

            /**
            * @brief Registers a new system within the SystemManager.
            *
            * @return std::shared_ptr<T> - Shared pointer to the newly registered system.
            */
            template <typename T>
            std::shared_ptr<T> registerSystem() {
                return _systemManager->registerSystem<T>();
            }

            /**
            * @brief Sets the signature for a system, defining which entities it will process.
            *
            * @param signature - The signature to associate with the system.
            */
            template <typename T>
            void setSystemSignature(Signature signature) {
                _systemManager->setSignature<T>(signature);
            }

            template<typename T, typename ClosureWeakPtr = std::shared_ptr<int>>
            int registerListener(std::shared_ptr<std::function<void(T&)>> listener, std::shared_ptr<ClosureWeakPtr> closure = nullptr)
            {
                return _eventManager->registerListener<T, ClosureWeakPtr>(listener, closure);
            }

            void unregisterListener(int listenerId)
            {
                _eventManager->unregisterListener(listenerId);
            }

            template <typename T>
            void emitEvent(T& event)
            {
                _eventManager->emitEvent<T>(event);
            }

            void dispatchEvents()
            {
                _eventManager->dispatchEvents();
            }

        private:
            std::shared_ptr<components::ComponentManager> _componentManager;
            std::shared_ptr<EntityManager> _entityManager;
            std::shared_ptr<system::SystemManager> _systemManager;
            std::shared_ptr<ecs::event::EventManager> _eventManager;
            std::shared_ptr<ecs::SingletonComponentManager> _singletonComponentManager;
    };
}