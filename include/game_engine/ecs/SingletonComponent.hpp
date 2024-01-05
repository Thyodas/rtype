/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** SingletonComponent.hpp
*/

#pragma once

#include <cstdint>
#include <cassert>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace ecs {
    class ISingletonComponent {
        public:
            virtual ~ISingletonComponent() = default;
    };

    template <typename T>
    class SingletonComponent : public ISingletonComponent {
        public:
            SingletonComponent(T instance) : _instance(instance) {};

            T &getInstance() {
                return _instance;
            }
        private:
            T _instance;
    };

    class SingletonComponentManager {
        public:
            template <typename T>
            void registerSingletonComponent(T component) {
                std::type_index typeName(typeid(T));
                assert(_singletonComponents.find(typeName) == _singletonComponents.end() && "Registering singleton component more than once.");
                _singletonComponents.insert({typeName, std::make_shared<SingletonComponent<T>>(std::move(component))});
            }

            template <typename T>
            T &getSingletonComponent() {
                std::type_index typeName(typeid(T));
                assert(_singletonComponents.find(typeName) != _singletonComponents.end() && "Singleton component not registered before use.");

                auto componentPtr = dynamic_cast<SingletonComponent<T>*>(_singletonComponents[typeName].get());
                assert(componentPtr && "Dynamic cast failed to convert to the required SingletonComponent type.");

                return componentPtr->getInstance();
            }

            template <typename T>
            void unregisterSingletonComponent() {
                std::type_index typeName(typeid(T));
                assert(_singletonComponents.find(typeName) != _singletonComponents.end() && "Singleton component not registered.");
                _singletonComponents.erase(typeName);
            }
        private:
            std::unordered_map<std::type_index, std::shared_ptr<ISingletonComponent>> _singletonComponents{};
    };
}