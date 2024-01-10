/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.hpp
*/

#pragma once

#include "my_raylib.h"
#include <string>

namespace ecs {
    namespace components {
        namespace sound {
            struct SoundData
            {
                Sound sound;
                bool isLooping;
                std::string name;
            };
            struct AudioSource
            {
                std::string filePath;
            };
        }
    }
}