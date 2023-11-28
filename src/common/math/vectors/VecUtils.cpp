/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** VecUtils.cpp
*/

#include "VecUtils.hpp"

namespace math {
    Vec2f normalize(const Vec2f &v)
    {
        float norm = v.x * v.x + v.y * v.y;
        if (norm > 0) {
            float invNorm = 1 / sqrtf(norm);
            return Vec2f(v.x * invNorm, v.y * invNorm);
        }
        return v;
    }
    float dotProduct(const Vec2f &v1, const Vec2f &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }
    Vec2f mix(const Vec2f &v1, const Vec2f &v2, const float &mixValue)
    {
        return v1 * (1 - mixValue) + v2 * mixValue;
    }

    Vec3f normalize(const Vec3f &v)
    {
        float norm = v.x * v.x + v.y * v.y + v.z * v.z;
        if (norm > 0) {
            float invNorm = 1 / sqrtf(norm);
            return Vec3f(v.x * invNorm, v.y * invNorm, v.z * invNorm);
        }
        return v;
    }
    float dotProduct(const Vec3f &v1, const Vec3f &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    Vec3f mix(const Vec3f &v1, const Vec3f &v2, const float &mixValue)
    {
        return v1 * (1 - mixValue) + v2 * mixValue;
    }

    Vec3f crossProduct(const Vec3f &v1, const Vec3f &v2)
    {
        return Vec3f(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }
}