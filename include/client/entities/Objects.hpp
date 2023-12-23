/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Objects
*/

#pragma once

#include "raymath.h"
#include "raylib.h"

namespace client {
    struct ObjectParams {
        Vector3 pos;
        float width;
        float height;
        float length;
        Color color;
        bool toggleWire;
        Color wireColor;
        Vector3 rotation;
        Vector3 scale;
    };

    enum class ObjectName : uint32_t {
        DualStriker,
        MicroRecon,
        RedFighter,
        UltravioletIntruder,
        GunBullet,
        None,
    };

    enum ObjectType {
        Cube,
        Model3D,
    };

    enum ObjectFormat {
        GLB,
        OBJ,
    };
}
