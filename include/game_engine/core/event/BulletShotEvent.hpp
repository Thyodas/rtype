/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** BulletShotEvent.hpp
*/

#pragma once

#include "./Event.hpp"
#include "game_engine/ecs/Entity.hpp"

class BulletShotEvent : public ecs::event::AEvent {
    public:
        BulletShotEvent() {}
};