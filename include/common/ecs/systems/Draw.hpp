/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Draw.hpp
*/

#pragma once

#include "../System.hpp"

namespace ecs {
    namespace system {
        class DrawSystemSFML : public System {
            public:
                void handleDraw();
        };
    }
}