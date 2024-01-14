/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "my_raylib.h"

#include "./Shapes.hpp"

#include <memory>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/SoftBody/SoftBodySharedSettings.h>
#include <Jolt/Physics/SoftBody/SoftBodyCreationSettings.h>
#include <Jolt/Physics/Collision/PhysicsMaterial.h>

namespace ecs {
    namespace components {
        namespace physics {

            enum BodyType {
                STATIC = 0,
                DYNAMIC = 1,
            };

            struct RigidBodyComponent {
                JPH::BodyID bodyID;
                JPH::Vec3 velocity;
                bool isAffectedByGravity;
                BodyType bodyType;

                RigidBodyComponent() : velocity(JPH::Vec3::sZero()), isAffectedByGravity(true), bodyType(DYNAMIC) {};
                RigidBodyComponent(float m, const JPH::Vec3& vel, bool gravity = true, BodyType type = DYNAMIC)
                    : velocity(vel), isAffectedByGravity(gravity), bodyType(type) {};
            };

            struct TransformComponent {
                JPH::Vec3 position;
                JPH::Quat rotation;
                JPH::Vec3 scale;

                TransformComponent() : position(JPH::Vec3::sZero()), rotation(JPH::Quat::sIdentity()), scale(JPH::Vec3::sReplicate(1.0f)) {};
                TransformComponent(const JPH::Vec3& pos, const JPH::Quat& rot, const JPH::Vec3& scl)
                    : position(pos), rotation(rot), scale(scl) {};
            };

            struct ColliderComponent {
                JPH::Ref<JPH::Shape> shape;

                explicit ColliderComponent() : shape(nullptr) {};
                explicit ColliderComponent(const JPH::Ref<JPH::Shape>& shp)
                    : shape(shp) {}
            };

            struct ForceComponent {
                JPH::Vec3 force;

                explicit ForceComponent() : force(JPH::Vec3::sZero()) {};
                explicit ForceComponent(const JPH::Vec3& f)
                    : force(f) {};
            };

            struct PhysicsMaterialComponent {
                std::shared_ptr<JPH::PhysicsMaterial> material;

                explicit PhysicsMaterialComponent() : material(nullptr) {};
                explicit PhysicsMaterialComponent(std::shared_ptr<JPH::PhysicsMaterial> mat)
                    : material(std::move(mat)) {};
            };
        }
    }
}
