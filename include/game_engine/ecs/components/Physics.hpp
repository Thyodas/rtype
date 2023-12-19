/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "common/math/vectors/Vec3.hpp"
#include "./Shapes.hpp"

#include "raylib.h"
#include <memory>

namespace ecs {
    namespace components {
        namespace physics {

            typedef struct transform_s {
                Vector3 pos;
                Vector3 rotation;
                Vector3 scale;
            } transform_t;

            typedef struct rigidBody_s {
                float mass;
                Vector3 velocity;
                Vector3 force;
            } rigidBody_t;

            enum class CollisionType {
                COLLIDE,
                LOOTABLE,
            };

            typedef struct collider_s {
                ShapeType shapeType;
                CollisionType collisionType;
                std::shared_ptr<IShape> data;
                BoundingBox box;
                Vector3 vertsGlobal[8];
                Matrix matRotate;
                Matrix matScale;
                Matrix matTranslate;
            } collider_t;

        }
    }
}