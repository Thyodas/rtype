/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Input.hpp
*/

#pragma once

#include "../System.hpp"
#include "../components/Input.hpp"
#include <map>

namespace ecs {
    namespace system {

        class InputSystem : public System {
            public:
                void handleInputs(void);
        };
    }
}