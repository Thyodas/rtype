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
#include "game_engine/GameEngine.hpp"

#include <algorithm>
#include <iostream>
#include <thread>



namespace ecs {
    namespace system {

        void JoltPhysicsSystem::update(JPH::PhysicsSystem &joltPhysicsSystem, float timeStep)
        {
            JPH::BodyInterface& bodyInterface = joltPhysicsSystem.GetBodyInterface();
            for (auto &entity : _entities) {
                std::cout << "dans la boucle" << std::endl;
                auto& rbComp = _coord->getComponent<components::physics::RigidBodyComponent>(entity);
                auto& transComp = _coord->getComponent<components::physics::TransformComponent>(entity);
                JPH::BodyID bodyID = rbComp.bodyID;

                if (bodyInterface.IsAdded(bodyID)) {
                    bodyInterface.SetPositionAndRotation(bodyID, transComp.position, transComp.rotation, JPH::EActivation::Activate);
                    bodyInterface.SetLinearVelocity(bodyID, rbComp.velocity);
                    std::cout << "on set dans le systeme" << std::endl;
                }
            }
            constexpr int cMaxPhysicsJobs = 2048;
            constexpr int cMaxPhysicsBarriers = 8;
            JPH::TempAllocatorImpl tempAllocator(10 * 1024 * 1024);
            JPH::JobSystemThreadPool jobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
            joltPhysicsSystem.Update(timeStep, 1, &tempAllocator, &jobSystemThreadPool);
            for (auto &entity : _entities) {
                auto& rbComp = _coord->getComponent<components::physics::RigidBodyComponent>(entity);
                auto& transComp = _coord->getComponent<components::physics::TransformComponent>(entity);
                JPH::BodyID bodyID = rbComp.bodyID;

                if (bodyInterface.IsAdded(bodyID)) {
                    transComp.position = bodyInterface.GetPosition(bodyID);
                    transComp.rotation = bodyInterface.GetRotation(bodyID);
                    rbComp.velocity = bodyInterface.GetLinearVelocity(bodyID);
                }
            }
        }
    }
}