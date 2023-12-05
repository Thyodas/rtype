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
                virtual BoundingBox getBoundingBox(Vector3 pos) const = 0;
                virtual void draw(Vector3 pos) const = 0;
        };

        class Cube : public IShape {
            public:
                Cube(float width = 2, float height = 2, float length = 2, bool toggleWire = false, Color color = RED, Color wireColor = BLACK) :
                    _width(width), _height(height), _length(length),
                    _toggleWire(toggleWire), _color(color),
                    _wireColor(wireColor) {}
                BoundingBox getBoundingBox(Vector3 pos) const override {
                    return (BoundingBox){(Vector3){pos.x - _width / 2,
                                                   pos.y - _height / 2,
                                                   pos.z - _length / 2},
                                         (Vector3){pos.x + _width / 2,
                                                   pos.y + _height / 2,
                                                   pos.z + _length / 2}};
                };
                void draw(Vector3 pos) const override {
                    DrawCube(pos, _width, _height, _length, _color);
                    if (_toggleWire)
                        DrawCubeWires(pos, _width, _height, _length, _wireColor);
                }
            private:
                float _width;
                float _height;
                float _length;
                bool _toggleWire;
                Color _color;
                Color _wireColor;
        };
    }
}