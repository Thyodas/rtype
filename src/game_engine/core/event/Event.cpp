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
                auto& type_index = _listeners.get<type>();
                auto range = type_index.equal_range(typeid(*event));
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->listener) {
                        (*(it->listener))(*event);
                    }
                }
                _eventQueue.pop();
            }
        }
    }
}