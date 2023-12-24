/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.hpp
*/

#pragma once

#include "../System.hpp"
#include "../Coordinator.hpp"

namespace ecs {
    namespace system {

        /**
         * @class BehaviourSystem
         * @brief System class responsible for handling and updating behaviour components in the ECS framework.
         *
         * Inherits from the base System class. This system is dedicated to managing and executing
         * behaviour logic for entities that possess behaviour components, similar to the way Unity handles
         * MonoBehaviour scripts.
         */
        class BehaviourSystem : public System {
            public:
                /**
                 * @brief Handles and updates the behaviours of all entities with behaviour components.
                 *
                 * This method iterates through all entities that have attached behaviour components
                 * and executes their update method.
                 */
                void handleBehaviours();
        };
    }
}
