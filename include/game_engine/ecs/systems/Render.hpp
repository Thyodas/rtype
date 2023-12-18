/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"
#include "../components/Render.hpp"
#include "../components/Physics.hpp"

#include <unordered_map>

namespace ecs {
    namespace system {
        class RenderSystem : public System {
        public:
            void render();
        };
    }
}
