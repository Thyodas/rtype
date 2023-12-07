/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** CollisionEvent.hpp
*/

#pragma once

#include "game_engine/ecs/event/Event.hpp"
#include "game_engine/ecs/Entity.hpp"

class CollisionEvent : public ecs::event::IEvent {
    public:
        CollisionEvent(ecs::Entity ent1, ecs::Entity ent2) : entity1(ent1), entity2(ent2) {
        }
        ecs::Entity entity1;
        ecs::Entity entity2;
};