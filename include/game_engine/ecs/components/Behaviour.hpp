/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Behaviour.hpp
*/

#pragma once

#include "./Physics.hpp"
#include "../Coordinator.hpp"
#include "../Entity.hpp"
#include <iostream>
#include <functional>
#include <memory>

namespace ecs {
    namespace components {
        namespace behaviour {
            class Behaviour {
                public:
                    virtual void update(void) = 0;
                    void setEntity(ecs::Entity entity)
                    {
                        _entity = entity;
                    }
                    static std::shared_ptr<ecs::Coordinator> _coord;
                protected:
                    ecs::Entity _entity;
            };
        }
    }
}