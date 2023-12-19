/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Objects
*/

#pragma once

// TODO: replace with own Vector3 / Color
#include "raymath.h"
#include "raylib.h"
//

namespace client {
    struct ObjectParams {
        Vector3 pos;
        float width;
        float height;
        float length;
        Color color;
        bool toggleWire;
        Color wireColor;
    };

    enum ObjectName {
        DualStriker,
        MicroRecon,
        RedFighter,
        UltravioletIntruder
    };

    enum ObjectType {
        Cube,
        Model3D,
    };
}
