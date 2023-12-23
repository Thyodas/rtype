/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Shapes.hpp
*/

#pragma once

#include "raylib.h"
#include "common/math/vectors/Vec3.hpp"
#include "common/math/vectors/Vec2.hpp"
#include "raymath.h"
#include <iostream>
#include <vector>

namespace ecs {
    namespace components {
        namespace physics {
            typedef struct transform_s transform_t;
            typedef struct collider_s collider_t;
        }
    }
}

namespace ecs {
    namespace components {
        enum class ShapeType {
            CUBE,
            LINE,
            POINT,
            CIRCLE,
            TRIANGLE,
            TRIANGLE_STRIP,
            SPHERE,
            CYLINDER,
            CAPSULE,
            PLANE,
            GRID,
            MODEL,
        };

        class IShape {
            public:
                ~IShape() = default;
                BoundingBox getBoundingBox(physics::collider_t &collider) const;
                virtual void draw(physics::transform_t &transf) const = 0;
                Model &getModel();
            protected:
                Model _model;
        };

        class Cube : public IShape {
            public:
                Cube(
                    float width = 2,
                    float height = 2,
                    float length = 2,
                    bool toggleWire = false,
                    Color color = RED,
                    Color wireColor = BLACK);
                void draw(physics::transform_t &transf) const override;
            private:
                float _width;
                float _height;
                float _length;
                bool _toggleWire;
                Color _color;
                Color _wireColor;
        };

        class Model3D : public IShape {
            public:
                Model3D(const char *filename, Color color = WHITE);
                void draw(physics::transform_t &transf) const override;
            private:
                Color _color;
        };

        class Skybox : public IShape {
            public:
                Skybox(const char *filename);
                void draw(physics::transform_t &transf) const override;
            private:
                Color _color;
        };
    }
}