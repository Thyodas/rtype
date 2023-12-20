/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity
*/

#include "client/entities/Entity.hpp"

client::Entity::Entity(ecs::Entity id) : _id(id)
{
}

client::Entity::~Entity()
{
}

client::behaviours client::Entity::getbehaviours() const
{
    return _behaviours;
}

void client::Entity::addBehaviour(std::string& name, ecs::components::behaviour::Behaviour& behaviour)
{
    _behaviours[name] = &behaviour;
}

void client::Entity::removeBehaviour(std::string& name)
{
    _behaviours.erase(name);
}

ecs::Entity client::Entity::getId() const
{
    return _id;
}
