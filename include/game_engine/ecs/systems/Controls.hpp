/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Controls.hpp
*/

#pragma once

#include "../System.hpp"

namespace ecs {
    namespace system {
            class ControlSystem : public System {
                public:
                    void updateControl();
                    void updateInputs();
            };
    }
}