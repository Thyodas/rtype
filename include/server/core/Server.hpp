/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Client
*/

#pragma once

#include "game_engine/core/Window.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"

#include "raylib.h"
#include <vector>

#include "server/core/NetServer.hpp"

namespace server {
    class Server {
        public:
            Server();
            ~Server() = default;

            void run();
        private:
            //std::vector<client::Entity> _entities;
            NetServer _netServer;

    };
}
