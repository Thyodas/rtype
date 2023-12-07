/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Event.hpp
*/

#pragma once

#include "raylib.h"
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <functional>
#include <queue>
#include <memory>

namespace ecs {
    namespace event {
        class IEvent{
            public:
                virtual ~IEvent() = default;
        };
        class EventManager {
            public:
                template<typename T>
                void registerListener(std::function<void(const T&)> listener)
                {
                    _listeners[typeid(T)].push_back(
                        [listener](const IEvent &event) {
                            listener(static_cast<const T&>(event));
                        }
                    );
                }

                template <typename T>
                void emitEvent(const T& event)
                {
                    _eventQueue.push(std::make_shared<T>(event));
                }

                void dispatchEvents()
                {
                    while (!_eventQueue.empty()) {
                        auto& event = _eventQueue.front();
                        auto& handlers = _listeners[typeid(*event)];
                        for (auto &handler : handlers) {
                            handler(*event);
                        }
                        _eventQueue.pop();
                    }
                }
            
            private:
                std::unordered_map<std::type_index, std::vector<std::function<void(const IEvent&)>>> _listeners;
                std::queue<std::shared_ptr<IEvent>> _eventQueue;
        };
    }
}