/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.cpp
*/

#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Coordinator.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void RenderSystem::render() {
            for (auto const &entity : _entities) {
                auto& transf = _coord->getComponent<components::physics::transform_t>(entity);
                auto& render = _coord->getComponent<components::render::render_t>(entity);
                auto& collision = _coord->getComponent<components::physics::collider_t>(entity);

                if (render.isRendered) {
                    if (render.type == components::ShapeType::MODEL) {
                        auto bb = GetMeshBoundingBox(render.data->getModel().meshes[0]);
                        // apply scale to bb
                        bb.min.x *= transf.scale.x;
                        bb.min.y *= transf.scale.y;
                        bb.min.z *= transf.scale.z;
                        bb.max.x *= transf.scale.x;
                        bb.max.y *= transf.scale.y;
                        bb.max.z *= transf.scale.z;
                        // apply change of position
                        bb.min.x += transf.pos.x;
                        bb.min.y += transf.pos.y;
                        bb.min.z += transf.pos.z;
                        bb.max.x += transf.pos.x;
                        bb.max.y += transf.pos.y;
                        bb.max.z += transf.pos.z;
                        collision.box = bb;
                        // change bb of model
                        DrawBoundingBox(bb, RED);
                    }
                    render.data->draw(transf);
                }
            }
        }
    }
}
