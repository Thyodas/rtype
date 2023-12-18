/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.cpp
*/

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/systems/Physics.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/core/event/CollisionEvent.hpp"

#include <algorithm>
#include <iostream>

namespace ecs {
    namespace system {
        void PhysicsSystem::updatePosition() {
            float deltaTime = 0.016f;
            for (auto const &entity : _entities) {
                auto& transf = _coord->getComponent<components::physics::transform_t>(entity);
                auto& body = _coord->getComponent<components::physics::rigidBody_t>(entity);

                transf.pos.x += body.velocity.x;
                body.velocity.x = 0;

                transf.pos.y += body.velocity.y;
                body.velocity.y = 0;

                transf.pos.z += body.velocity.z;
                body.velocity.z = 0;


                // transf.vel.x += transf.acc.x * deltaTime;
                // transf.vel.y += transf.acc.y * deltaTime;
                // transf.vel.z += transf.acc.z * deltaTime;

                // transf.pos.x += transf.vel.x * deltaTime;
                // transf.pos.y += transf.vel.y * deltaTime;
                // transf.pos.z += transf.vel.z * deltaTime;

                // transf.vel.x *= 0.7f;
                // transf.vel.y *= 0.7f;
                // transf.vel.z *= 0.7f;

            }
        }

        void ColisionDetectionSystem::detectSphereCollision(ecs::components::physics::collider_t data1,
                                                            ecs::components::physics::collider_t data2) {

        }

        void ColisionDetectionSystem::detectSphereBoxCollision(ecs::components::physics::collider_t collider1,
                                                            ecs::components::physics::collider_t collider2)
        {

        }

        void ColisionDetectionSystem::detectCollision() {
            for (auto it1 = _entities.begin(); it1 != _entities.end(); it1++) {
                auto& transf1 = _coord->getComponent<components::physics::transform_t>(*it1);
                auto& collider1 = _coord->getComponent<components::physics::collider_t>(*it1);
                for (auto it2 = it1; it2 != _entities.end();) {
                    it2++;
                    if (it2 == _entities.end()) break;
                    auto& transf2 = _coord->getComponent<components::physics::transform_t>(*it2);
                    auto& collider2 = _coord->getComponent<components::physics::collider_t>(*it2);
                    if (collider1.shapeType == ecs::components::ShapeType::SPHERE || collider2.shapeType == ecs::components::ShapeType::SPHERE) {
                        
                    } else {
                        BoundingBox box1 = collider1.data->getBoundingBox(transf1);
                        BoundingBox box2 = collider2.data->getBoundingBox(transf2);
                        bool colliding = CheckCollisionBoxes(box1, box2);
                        if (colliding) {
                            _coord->emitEvent<CollisionEvent>(CollisionEvent(*it1, box1, collider1.data->getModel().transform, *it2, box2, collider2.data->getModel().transform));
                        } else {
                            //std::cout << "no collision detected" << std::endl;
                        }
                    }
                }
            }
        }

        CollisionResponse::CollisionResponse(ecs::Coordinator &coord) : _coord(coord)
        {
            _coord.registerListener<CollisionEvent>([this](const CollisionEvent &event) {
                auto &transf = _coord.getComponent<ecs::components::physics::transform_t>(event.entity1);
                Vector3 displacementVector = getCollisionResponse(event);
                transf.pos.x += displacementVector.x;
                transf.pos.y += displacementVector.y;
                transf.pos.z += displacementVector.z;
            });
        }

        float CollisionResponse::getOverlap(Vector2 a, Vector2 b)
        {
            if (a.x > b.y)
                return 0;
            if (b.x > a.y)
                return 0;
            if (a.x > b.x)
                return b.y - a.x;
            return b.x - a.y;
        }

        void CollisionResponse::getCollisionVectors(const Matrix rotate1, const Matrix rotate2, Vector3 *vecs)
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

        Vector2 CollisionResponse::getColliderProjectionBounds(const BoundingBox &box, Vector3 vec)
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

        Vector3 CollisionResponse::getCollisionResponse(const CollisionEvent &event)
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
    }
}