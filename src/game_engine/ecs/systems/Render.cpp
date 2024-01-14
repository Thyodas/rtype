/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.cpp
*/

#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "common/utils/Math.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void RenderSystem::render() {
            for (auto const &entity : _entities) {
                auto& transf = _coord->getComponent<ecs::components::physics::TransformComponent>(entity);
                auto& render = _coord->getComponent<components::render::render_t>(entity);
                Vector3 scale = common::utils::joltVectorToRayVector(transf.scale);
                Quaternion rotate = common::utils::joltQuatToRayQuat(transf.rotation);
                Matrix rotateMatrix = QuaternionToMatrix(rotate);
                Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);
                render.data->getModel().transform = MatrixMultiply(scaleMatrix, rotateMatrix);

                if (render.isRendered) {
                    render.data->draw(transf);
                }
            }
        }
    }
}
