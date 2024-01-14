/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Shapes.cpp
*/

#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "rlgl.h"
#include "common/utils/Math.hpp"

namespace ecs {
    namespace components {
        Model &IShape::getModel()
        {
            return _model;
        }

        Cube::Cube(
            float width,
            float height,
            float length,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _width(width),
            _height(height),
            _length(length),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            _model = LoadModelFromMesh(GenMeshCube(_width, _height, _length));
        }

        void Cube::draw(ecs::components::physics::TransformComponent &transf) const
        {
            DrawModel(_model, common::utils::joltVectorToRayVector(transf.position), 1, _color);
            //DrawBoundingBox(getBoundingBox(transf), WHITE);
            if (_toggleWire)
                DrawModelWires(_model, common::utils::joltVectorToRayVector(transf.position), 1, _wireColor);
        }

        Model3D::Model3D(const char *filename, Color color)
        {
            _model = LoadModel(filename);
            _color = color;
        }

        void Model3D::draw(ecs::components::physics::TransformComponent &transf) const
        {
            DrawModel(_model, common::utils::joltVectorToRayVector(transf.position), 1, _color);
            DrawBoundingBox(GetModelBoundingBox(_model), RED);
        }

        Skybox::Skybox(const char *filename)
        {
            Texture2D texture = LoadTexture(filename);
            Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
            _model = LoadModelFromMesh(cube);
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        }

        void Skybox::draw(ecs::components::physics::TransformComponent &transf) const
        {
            rlDisableBackfaceCulling();
            rlDisableDepthMask();

            DrawModel(_model, common::utils::joltVectorToRayVector(transf.position) , 50.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
        }
    }
}