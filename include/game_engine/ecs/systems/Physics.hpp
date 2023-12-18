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
#include "game_engine/core/event/CollisionEvent.hpp"

namespace ecs {
    namespace system {
        class PhysicsSystem : public System {
            public:
                void updatePosition();
        };
        class ColisionDetectionSystem : public System {
            public:
                void detectCollision();
            private:
                void detectSphereCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
                void detectSphereBoxCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
        };

        class CollisionResponse {
            public:
                CollisionResponse(ecs::Coordinator &coord);
                Vector3 getCollisionResponse(const CollisionEvent &event);

            private:
                float getOverlap(Vector2 a, Vector2 b);
                void getCollisionVectors(const Matrix rotate1, const Matrix rotate2, Vector3 *vecs);
                Vector2 getColliderProjectionBounds(const BoundingBox &box, Vector3 vec);

                ecs::Coordinator& _coord;
        };
    }
}