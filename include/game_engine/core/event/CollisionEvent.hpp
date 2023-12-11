/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** CollisionEvent.hpp
*/

#pragma once

#include "./Event.hpp"
#include "game_engine/ecs/Entity.hpp"

class CollisionEvent : public ecs::event::IEvent {
    public:
        CollisionEvent(
            ecs::Entity ent1,
            BoundingBox &box1,
            Matrix &rotate1,
            ecs::Entity ent2,
            BoundingBox &box2,
            Matrix &rotate2
        ) :
            entity1(ent1),
            box1(box1),
            rotate1(rotate1),
            entity2(ent2),
            box2(box2),
            rotate2(rotate2) {
        }
        ecs::Entity entity1;
        BoundingBox box1;
        Matrix rotate1;
        ecs::Entity entity2;
        BoundingBox box2;
        Matrix rotate2;
};