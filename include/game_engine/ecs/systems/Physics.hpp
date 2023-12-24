/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"
#include "../components/Physics.hpp"
#include "../../core/event/CollisionEvent.hpp"

namespace ecs {
    namespace system {
        /**
         * @class PhysicsSystem
         * @brief System responsible for updating the positions of entities based on their physical properties.
         *
         * Inherits from the base System class. This system updates the position of entities in the game world,
         * taking into account their current velocities, forces, and other physical attributes.
         */
        class PhysicsSystem : public System {
            public:
            /**
                 * @brief Updates the position of all entities with physics components.
                 *
                 * This method calculates and applies the necessary positional changes to entities based on their
                 * physics attributes such as velocity and applied forces.
                 */
                void updatePosition();
        };

        /**
         * @class CollisionDetectionSystem
         * @brief System responsible for detecting collisions between entities in the game world.
         *
         * Inherits from the base System class. This system checks for and identifies collisions between entities
         * using their collider components. It is capable of handling different types of collisions like sphere-sphere
         * and sphere-box collisions.
         */
        class ColisionDetectionSystem : public System {
            public:
            /**
                 * @brief Detects collisions between entities and triggers collision events.
                 *
                 * This method iterates through entities with collider components and checks for collisions,
                 * triggering collision events when they occur.
                 */
                void detectCollision();
            private:
                void detectSphereCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
                void detectSphereBoxCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
        };

        /**
         * @class CollisionResponse
         * @brief Handles the response to collisions between entities.
         *
         * This class is responsible for calculating and applying the appropriate response to collisions detected
         * by the CollisionDetectionSystem. It includes methods for determining collision responses and updating
         * collider vertices.
         */
        class CollisionResponse {
            public:
                /**
                 * @brief Constructs a CollisionResponse object with a reference to the ECS coordinator.
                 * @param coord Reference to the ECS coordinator.
                 */
                CollisionResponse(ecs::Coordinator &coord);
                /**
                 * @brief Calculates the response to a collision event.
                 * @param event The collision event to be handled.
                 * @return The calculated collision response as a Vector3.
                 */
                Vector3 getCollisionResponse(const CollisionEvent &event);

                /**
                 * @brief Updates the global vertices of a collider.
                 * @param collider The collider to update.
                 */
                static void updateColliderGlobalVerts(ecs::components::physics::collider_t &collider);

            private:
                float getOverlap(Vector2 a, Vector2 b);
                void getCollisionVectors(
                    ecs::components::physics::collider_t &collider1,
                    ecs::components::physics::collider_t &collider2,
                    Vector3 *vecs
                );
                Vector2 getColliderProjectionBounds(
                    ecs::components::physics::collider_t &collider1,
                    Vector3 vec
                );

                ecs::Coordinator& _coord;
        };
    }
}