/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Event.cpp
*/

#include "game_engine/core/event/Event.hpp"

namespace ecs {
    namespace event {

        void EventManager::dispatchEvents()
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
    }
}