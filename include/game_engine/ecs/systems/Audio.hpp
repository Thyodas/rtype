/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "my_raylib.h"
#include "../System.hpp"
#include "../components/Audio.hpp"

#include <vector>
#include <string>

namespace ecs {
    namespace system {
        class AudioSystem : public System {
            public:
                void update();
        };
        class MusicSystem : public System {
            public:
                void update();
        };
    }
}
