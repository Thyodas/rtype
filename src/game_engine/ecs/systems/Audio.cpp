/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.cpp
*/

#include "game_engine/ecs/systems/Audio.hpp"
#include "game_engine/ecs/components/Audio.hpp"

#include <iostream>

using namespace ecs::system;
using ecs::components::sound::SoundData;

AudioSystem::AudioSystem() {
    // initialize();
}

AudioSystem::~AudioSystem() {
    for (auto& soundData : _sounds) {
        UnloadSound(soundData.sound);
    }
    CloseAudioDevice();
}

void AudioSystem::initialize() {
    InitAudioDevice();
}

void AudioSystem::addSound(const std::string& soundFile, const std::string& name) {
    SoundData newSound;
    newSound.sound = LoadSound(soundFile.c_str());
    if (newSound.sound.stream.buffer == NULL) {
        return;
    }
    newSound.name = name;
    _sounds.push_back(newSound);
}

void AudioSystem::playSound(const std::string& name, bool loop) {
    for (auto& soundData : _sounds) {
        if (soundData.name == name) {
            soundData.isLooping = loop;
            PlaySound(soundData.sound);
            break;
        }
    }
}

void AudioSystem::playSoundFromPath(const std::string& path) {
    Sound sound = LoadSound(path.c_str());
    if (sound.stream.buffer == NULL) {
        return;
    }
    PlaySound(sound);
}

void AudioSystem::update() {
    for (auto& soundData : _sounds) {
        if (soundData.isLooping && !IsSoundPlaying(soundData.sound)) {
            PlaySound(soundData.sound);
        }
    }
}
