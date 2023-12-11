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
#include "../../../../src/game_engine/core/event/CollisionEvent.hpp"

namespace ecs {
    namespace system {
        class PhysicsSystem : public System {
            public:
                void updatePosition(ecs::Coordinator &coord);
        };
        class ColisionDetectionSystem : public System {
            public:
                void detectCollision(ecs::Coordinator &coord);
            private:
                void detectSphereCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
                void detectSphereBoxCollision(ecs::components::physics::collider_t collider1, ecs::components::physics::collider_t collider2);
        };

        class CollisionResponse {
            public:

                float getOverlap(Vector2 a, Vector2 b)
                {
                    if (a.x > b.y)
                        return 0;
                    if (b.x > a.y)
                        return 0;
                    if (a.x > b.x)
                        return b.y - a.x;
                    return b.x - a.y;
                }

                void getCollisionVectors(const Matrix rotate1, const Matrix rotate2, Vector3 *vecs)
                {
                    Vector3 x = {1, 0, 0};
                    Vector3 y = {0, 1, 0};
                    Vector3 z = {0, 0, 1};

                    vecs[0] = Vector3Transform(x, rotate1);
                    vecs[1] = Vector3Transform(y, rotate1);
                    vecs[2] = Vector3Transform(z, rotate1);

                    vecs[3] = Vector3Transform(x, rotate2);
                    vecs[4] = Vector3Transform(y, rotate2);
                    vecs[5] = Vector3Transform(z, rotate2);

                    int i = 6;
                    for (int j = 0; j < 3; ++j) {
                        for (int k = 3; k < 6; ++k) {
                            if (Vector3Equals(vecs[j], vecs[k]))
                                vecs[i] = x;
                            else
                                vecs[i] = Vector3Normalize(Vector3CrossProduct(vecs[j], vecs[k]));
                            i++;
                        }
                    }
                }

                Vector2 getColliderProjectionBounds(const BoundingBox &box, Vector3 vec)
                {
                    Vector2 bounds = {0};
                    Vector3 vertsGlobal[8];
                    vertsGlobal[0] = (Vector3) { box.min.x, box.min.y, box.min.z };
                    vertsGlobal[1] = (Vector3) { box.min.x, box.min.y, box.max.z };
                    vertsGlobal[2] = (Vector3) { box.min.x, box.max.y, box.min.z };
                    vertsGlobal[3] = (Vector3) { box.min.x, box.max.y, box.max.z };
                    vertsGlobal[4] = (Vector3) { box.max.x, box.min.y, box.min.z };
                    vertsGlobal[5] = (Vector3) { box.max.x, box.min.y, box.max.z };
                    vertsGlobal[6] = (Vector3) { box.max.x, box.max.y, box.min.z };
                    vertsGlobal[7] = (Vector3) { box.max.x, box.max.y, box.max.z };

                    float proj = Vector3DotProduct(vertsGlobal[0], vec);
                    bounds.x = bounds.y = proj;
                    for (int i = 1; i < 8; ++i) {
                        proj = Vector3DotProduct(vertsGlobal[i], vec);
                        bounds.x = fmin(bounds.x, proj);
                        bounds.y = fmax(bounds.y, proj);
                    }
                    return bounds;
                }

                Vector3 getCollisionResponse(const CollisionEvent &event)
                {
                    float overlapMin = 100.f;
                    Vector3 overlapDir = {0};

                    Vector3 testVec[15];
                    getCollisionVectors(event.rotate1, event.rotate2, testVec);
                    for (int i = 0; i < 15; ++i) {
                        Vector2 apro, bpro;
                        apro = getColliderProjectionBounds(event.box1, testVec[i]);
                        bpro = getColliderProjectionBounds(event.box2, testVec[i]);

                        float overlap = getOverlap(apro, bpro);
                        if (overlap == 0)
                            return Vector3Zero();
                        if (fabs(overlap) < fabs(overlapMin)) {
                            overlapMin = overlap;
                            overlapDir = testVec[i];
                        }
                    }
                    return Vector3Scale(overlapDir, overlapMin);
                }
                CollisionResponse(ecs::Coordinator &coord) : _coord(coord) {
                    _coord.registerListener<CollisionEvent>([this](const CollisionEvent &event) {
                        std::cout << "collision between " << event.entity1 << " and " << event.entity2 << std::endl;
                        auto &transf = _coord.getComponent<ecs::components::physics::transform_t>(event.entity1);
                        Vector3 displacementVector = getCollisionResponse(event);
                        transf.pos.x += displacementVector.x;
                        transf.pos.y += displacementVector.y;
                        transf.pos.z += displacementVector.z;
                    });
                };
            private:
                ecs::Coordinator& _coord;
        };
    }
}