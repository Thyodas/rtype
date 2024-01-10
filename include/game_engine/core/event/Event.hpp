/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Event.hpp
*/

#pragma once

#include "my_raylib.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <functional>
#include <queue>
#include <memory>

namespace ecs {
    namespace event {
        /**
         * @class AEvent
         * @brief Interface class representing a generic event in the ECS system.
         *
         * This class serves as a base for all events within the ECS framework. It allows
         * for polymorphic handling of different event types without knowing their specific
         * classes.
         */
        class AEvent {
            public:
                virtual ~AEvent() = default;

                bool isConsumed = false;
        };

        struct ListenerRecord {
            int id;
            std::type_index type;
            std::shared_ptr<std::function<void(AEvent&)>> listener;

            ListenerRecord(int id, std::type_index type, std::shared_ptr<std::function<void(AEvent&)>> listener)
                : id(id), type(type), listener(std::move(listener)) {}
        };

        struct id {};
        struct type {};

        using namespace boost::multi_index;

        typedef multi_index_container<
            ListenerRecord,
            indexed_by<
                ordered_unique<tag<id>, member<ListenerRecord, int, &ListenerRecord::id>>,
                ordered_non_unique<tag<type>, member<ListenerRecord, std::type_index, &ListenerRecord::type>>
            >
        >ListenerSet;

        /**
         * @class EventManager
         * @brief Manages event handling and dispatching in the ECS system.
         *
         * This class is responsible for registering event listeners, emitting events, and
         * managing the event dispatch process. It allows for loose coupling between
         * different components of the ECS system by using an event-driven approach.
         */
        class EventManager {
            public:
                EventManager() : _nextId(1) {}

                /**
                 * @brief Registers a listener for a specific event type.
                 * @param listener The function to be called when the event of type T occurs.
                 */
                template<typename T, typename ClosureWeakPtr = std::shared_ptr<int>>
                int registerListener(std::shared_ptr<std::function<void(T&)>> listener, std::shared_ptr<ClosureWeakPtr> closure = nullptr)
                {
                    int id = _nextId++;
                    auto wrapper = std::make_shared<std::function<void(AEvent&)>>([listener](AEvent &event) -> void {
                        (*listener)(static_cast<T&>(event));
                    });
                    _listeners.insert(ListenerRecord(id, typeid(T), wrapper));
                    return id;
                }

                void unregisterListener(int listenerId)
                {
                    auto& id_index = _listeners.get<id>();
                    id_index.erase(listenerId);
                }

                void unregisterListener(int listenerId)
                {
                    auto& id_index = _listeners.get<id>();
                    id_index.erase(listenerId);
                }

                /**
                 * @brief Emits an event of a specific type, adding it to the event queue.
                 * @param event The event of type T to be emitted.
                 */
                template <typename T>
                void emitEvent(T& event)
                {
                    _eventQueue.push(std::make_shared<T>(event));
                }

                /**
                 * @brief Dispatches all events in the event queue to their respective listeners.
                 *
                 * This method goes through the event queue, dispatching each event to the
                 * registered listeners for its type. It should be called regularly to ensure
                 * timely handling of events.
                 */
                void dispatchEvents();

            private:
                ListenerSet _listeners;
                std::queue<std::shared_ptr<AEvent>> _eventQueue;
                int _nextId;
        };
    }
}
