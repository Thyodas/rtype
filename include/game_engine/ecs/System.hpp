/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** System.hpp
*/

#pragma once

#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Entity.hpp"
#include "Signature.hpp"

namespace ecs {
    class Coordinator;
}

namespace ecs {
    namespace system {
        /**
        * @class System
        *
        * @brief Base class for systems in an ECS (Entity-Component-System) architecture.
        *
        * Systems are responsible for processing entities that have a specific set of components.
        * This class provides the basic structure for a system, mainly a set of entities that the system will process.
        */
        class System {
            public:
                std::set<Entity> _entities;
                static std::shared_ptr<ecs::Coordinator> _coord;
        };

        /**
        * @class SystemManager
        *
        * @brief Manages systems in the ECS architecture.
        *
        * This class is responsible for registering systems, setting their signatures,
        * and updating systems with relevant entities based on entity signature changes.
        */
        class SystemManager {
            public:
                /**
                * @brief Registers a new system of type T in the ECS framework.
                *
                * @tparam T - The type of the system to be registered.
                * @return std::shared_ptr<T> - Shared pointer to the newly registered system.
                */
                template<typename T>
                std::shared_ptr<T> registerSystem() {
                    std::type_index typeName(typeid(T));

                    assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

                    auto system = std::make_shared<T>();
                    _systems.insert({typeName, system});
                    return system;
                }

                /**
                * @brief Sets the signature for a system.
                *
                * The signature determines which entities the system will process based on their component makeup.
                * @tparam T - The type of the system for which the signature is being set.
                * @param signature - The signature to associate with the system.
                */
                template<typename T>
                void setSignature(Signature signature) {
                    std::type_index typeName(typeid(T));

                    assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

                    _signatures.insert({typeName, signature});
                }

                /**
                * @brief Handles the destruction of an entity by removing it from all systems.
                *
                * @param entity - The ID of the destroyed entity.
                */
                void entityDestroyed(Entity entity) {
                    for (auto const &pair : _systems) {
                        auto const &system = pair.second;
                        system->_entities.erase(entity);
                    }
                }

                /**
                * @brief Updates the systems with an entity when its signature changes.
                *
                * This ensures that systems process only relevant entities based on their current components.
                * @param entity - The ID of the entity whose signature has changed.
                * @param entitySignature - The new signature of the entity.
                */
                void entitySignatureChanged(Entity entity, Signature entitySignature) {
                    for (auto const &pair : _systems) {
                        auto const &type = pair.first;
                        auto const &system = pair.second;
                        auto const &systemSignature = _signatures[type];

                        if ((entitySignature & systemSignature) == systemSignature)
                            system->_entities.insert(entity);
                        else
                            system->_entities.erase(entity);
                    }
                }
            private:
                std::unordered_map<std::type_index, Signature> _signatures{};
                std::unordered_map<std::type_index, std::shared_ptr<System>> _systems{};
        };
    }
}