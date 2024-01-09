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
                std::vector<decltype(handlers.begin())> toRemove;
                for (auto it = handlers.begin(); it != handlers.end(); ++it) {
                    if (event->isConsumed) {
                        break;
                    }
                    if ((*it)(*event)) {
                        toRemove.push_back(it);
                    }
                }
                for (auto& it : toRemove) {
                    handlers.erase(it);
                }
                _eventQueue.pop();
            }
        }
    }
}