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
        void PhysicsSystem::updatePosition(ecs::Coordinator &coord) {
            float deltaTime = 0.016f;
            for (auto const &entity : _entities) {
                auto& transf = coord.getComponent<components::physics::transform_t>(entity);
                auto& body = coord.getComponent<components::physics::rigidBody_t>(entity);

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

        void ColisionDetectionSystem::detectCollision(ecs::Coordinator &coord) {
            for (auto it1 = _entities.begin(); it1 != _entities.end(); it1++) {
                auto& transf1 = coord.getComponent<components::physics::transform_t>(*it1);
                auto& collider1 = coord.getComponent<components::physics::collider_t>(*it1);
                for (auto it2 = it1; it2 != _entities.end();) {
                    it2++;
                    if (it2 == _entities.end()) break;
                    auto& transf2 = coord.getComponent<components::physics::transform_t>(*it2);
                    auto& collider2 = coord.getComponent<components::physics::collider_t>(*it2);
                    if (collider1.shapeType == ecs::components::ShapeType::SPHERE || collider2.shapeType == ecs::components::ShapeType::SPHERE) {
                        
                    } else {
                        BoundingBox box1 = collider1.data->getBoundingBox(transf1.pos);
                        BoundingBox box2 = collider2.data->getBoundingBox(transf2.pos);
                        bool colliding = CheckCollisionBoxes(box1, box2);
                        if (colliding) {
                            coord.emitEvent<CollisionEvent>(CollisionEvent(*it1, box1, collider1.data->getModel().transform, *it2, box2, collider2.data->getModel().transform));
                        } else {
                            //std::cout << "no collision detected" << std::endl;
                        }
                    }
                }
            }
        }
    }
}