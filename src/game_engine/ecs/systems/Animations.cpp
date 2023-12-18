/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Animations.cpp
*/

#include "game_engine/ecs/systems/Animations.hpp"
#include "game_engine/ecs/components/Animations.hpp"

namespace ecs {
    namespace system {
        void AnimationSystem::handleAnimations(ecs::Coordinator &coord) {
            for (auto const &entity : _entities) {
                auto &anim = coord.getComponent<ecs::components::animations::animation_t>(entity);

                anim.animFrameCounter++;
                UpdateModelAnimation(anim.model, anim.anims[0], anim.animFrameCounter);
                if (anim.animFrameCounter >= anim.anims[0].frameCount)
                    anim.animFrameCounter = 0;
            }
        }
    }
}