/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EnemyDestroyEvent.hpp
*/

#pragma once

#include "./Event.hpp"
#include "game_engine/ecs/Entity.hpp"

class EnemyDestroyEvent : public ecs::event::AEvent {
    public:
        EnemyDestroyEvent(ecs::Entity entity) : entity(entity)
        {
        }

        ecs::Entity entity;
};
