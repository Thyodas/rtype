/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Jolt.hpp
*/

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace common::utils {

    /**
     * @brief Create a jolt shapes based on the scale factor (used when the user scaled the body)
     *
     * @param scale
     * @param shapeType
     * @return JPH::Ref<JPH::Shape>
     */
    JPH::Ref<JPH::Shape> createShapeWithScale(const JPH::Vec3& scale, JPH::ShapeRefC &shape)
    {
        JPH::EShapeSubType shapeType = shape->GetSubType();
        switch(shapeType) {
            case JPH::EShapeSubType::Box:
                const JPH::BoxShape* boxShape = dynamic_cast<const JPH::BoxShape*>(shape.GetPtr());
                JPH::Vec3 halfExtents = boxShape->GetHalfExtent();
                halfExtents = halfExtents * scale;
                JPH::Ref<JPH::Shape> scaledBoxShape = new JPH::BoxShape(halfExtents, 0.0f);
                return scaledBoxShape;
        }
    }
}