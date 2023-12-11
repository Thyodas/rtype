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
                BoundingBox getBoundingBox(Vector3 pos) const
                {
                    BoundingBox box = GetModelBoundingBox(_model);
                    box.min = Vector3Transform(box.min, _model.transform);
                    box.max = Vector3Transform(box.max, _model.transform);
                    Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
                    box.min = Vector3Transform(box.min, translation);
                    box.max = Vector3Transform(box.max, translation);
                    return box;
                };
                virtual void draw(Vector3 pos) const = 0;
                virtual Model &getModel() = 0;
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
                    Color wireColor = BLACK) :
                    _width(width),
                    _height(height),
                    _length(length),
                    _toggleWire(toggleWire),
                    _color(color),
                    _wireColor(wireColor)
                {
                        _model = LoadModelFromMesh(GenMeshCube(_width, _height, _length));
                }
                void draw(Vector3 pos) const override
                {
                    DrawModel(_model, pos, 1, _color);
                    DrawBoundingBox(getBoundingBox(pos), WHITE);
                    if (_toggleWire)
                        DrawModelWires(_model, pos, 1, _wireColor);
                }

                Model &getModel() override
                {
                    return _model;
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