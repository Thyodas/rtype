/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.hpp
*/

#pragma once

#include "./Physics.hpp"
#include <iostream>

namespace ecs {
    namespace components {
        namespace behaviour {
            class Behaviour {
                public:
                    virtual void update(physics::transform_t &transf) {
                        std::cout << "ici" << std::endl;
                    };
            };
        }
    }
}