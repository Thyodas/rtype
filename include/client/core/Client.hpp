/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#pragma once

#include "raylib.h"
#include "game_engine/core/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"

namespace client {
    class Client {
        public:
            Client();
            ~Client();

            void run();

        protected:
        private:
            // Liste d'entités
    };
}
