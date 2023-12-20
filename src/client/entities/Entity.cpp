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

client::behaviors client::Entity::getbehaviors() const
{
    return _behaviors;
}

void client::Entity::addBehavior(std::string& name, ecs::components::behavior::Behavior& behavior)
{
    _behaviors[name] = behavior;
}

void client::Entity::removeBehavior(std::string& name)
{
    _behaviors.erase(name);
}

ecs::Entity client::Entity::getId() const
{
    return _id;
}
