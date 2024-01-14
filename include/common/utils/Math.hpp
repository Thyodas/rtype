/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Math.hpp
*/

#pragma once

#include "raylib.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

namespace common::utils {

    /**
     * @brief Converts a 3D vector from the jolt library to a 3D vector of the raylib
     *
     * @param inVec
     * @return Vector3
     */
    static Vector3 joltVectorToRayVector(JPH::Vec3 inVec)
    {
        return Vector3{inVec.GetX(), inVec.GetY(), inVec.GetZ()};
    }

    /**
     * @brief Converts a 3D vector from the raylib to a 3D vector of the Jolt library
     *
     * @param inVec
     * @return JPH::Vec3
     */
    static JPH::Vec3 rayVectorToJoltVector(Vector3 inVec)
    {
        return JPH::Vec3(inVec.x, inVec.y, inVec.z);
    }

    /**
     * @brief Converts a quaternion from the jolt library to a quaternion of the raylib
     *
     * @param inQuat
     * @return Quaternion
     */
    static Quaternion joltQuatToRayQuat(JPH::Quat inQuat)
    {
        return Quaternion{inQuat.GetX(), inQuat.GetY(), inQuat.GetZ(), inQuat.GetW()};
    }

    /**
     * @brief Converts a quaternion from the raylib to a quaternion of the jolt library
     *
     * @param inQuat
     * @return JPH::Quat
     */
    static JPH::Quat rayQuatToJoltQuat(Quaternion inQuat)
    {
        return JPH::Quat(inQuat.x, inQuat.y, inQuat.z, inQuat.w);
    }

    static JPH::Ref<JPH::BoxShape> convertBoundingBoxToBoxShape(const BoundingBox &raylibBox)
    {
        JPH::Vec3 extents(
            (raylibBox.max.x - raylibBox.min.x) * 0.5f,
            (raylibBox.max.y - raylibBox.min.y) * 0.5f,
            (raylibBox.max.z - raylibBox.min.z) * 0.5f
        );
        JPH::Ref<JPH::BoxShape> boxShape = new JPH::BoxShape(extents, 0);
        return boxShape;
    }
}