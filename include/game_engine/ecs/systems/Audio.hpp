/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.hpp
*/

#pragma once

#include "my_raylib.h"
#include "../System.hpp"
#include "../components/Audio.hpp"

#include <vector>
#include <string>

// struct PlayerActionEvent {
//     bool spacePressed;
// };

namespace ecs {
    namespace system {

        class AudioSystem : public System {
            private:
                std::vector<ecs::components::sound::SoundData> _sounds;

            public:
                AudioSystem();
                ~AudioSystem();

                void initialize();
                void addSound(const std::string& soundFile, const std::string& name);
                void playSound(const std::string& name, bool loop = false);
                void playSoundFromPath(const std::string& path);
                void update();
        };
    }
}
