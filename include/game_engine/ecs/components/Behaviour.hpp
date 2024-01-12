/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.hpp
*/

#pragma once

#include "./Physics.hpp"
#include "../Coordinator.hpp"
#include "../Entity.hpp"
#include <iostream>
#include <functional>
#include <memory>

namespace ecs {
    namespace components {
        namespace behaviour {
            /**
             * @class Behaviour
             * @brief Abstract class representing a behaviour component, akin to Unity's behaviour scripts.
             *
             * This class provides a base for creating behaviour components in an ECS (Entity Component System)
             * similar to Unity's MonoBehaviour. Each behaviour should derive from this class and implement
             * the update method. Behaviours are attached to entities and are used to define their actions
             * and reactions within the game world.
             */
            class Behaviour {
                public:
                    virtual ~Behaviour() = default;

                    /**
                     * @brief Abstract method to be implemented by derived classes for updating the behaviour.
                     *
                     * This method is called every frame and should contain the logic for the behaviour's
                     * action and interaction within the game world.
                     */
                    virtual void update(void) = 0;
                    /**
                     * @brief Sets the entity associated with this behaviour.
                     * @param entity The entity to which this behaviour is attached.
                     */
                    virtual void setEntity(ecs::Entity entity);

                    static std::shared_ptr<ecs::Coordinator> _coord; ///< Shared pointer to the ECS coordinator.
                protected:
                    ecs::Entity _entity;
            };
        }
    }
}