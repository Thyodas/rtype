/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity
*/

#include "common/game/entities/Entity.hpp"

namespace common::game {
    Entity::Entity(ecs::Entity id) : _id(id)
    {
    }

    Entity::~Entity()
    {
    }

    behaviours Entity::getbehaviours() const
    {
        return _behaviours;
    }

    void Entity::addBehaviour(std::string& name, ecs::components::behaviour::Behaviour& behaviour)
    {
        _behaviours[name] = &behaviour;
    }

    void Entity::removeBehaviour(std::string& name)
    {
        _behaviours.erase(name);
    }

    ecs::Entity Entity::getId() const
    {
        return _id;
    }
}
