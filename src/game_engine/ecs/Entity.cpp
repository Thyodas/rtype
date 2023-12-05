/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity.cpp
*/

#include "game_engine/ecs/Entity.hpp"

namespace ecs {
    EntityManager::EntityManager() {
        for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
            _availableEntities.push(entity);
    }

    Entity EntityManager::createEntity() {
        assert(_livingEntityCount < MAX_ENTITIES && "Too many living entities");

        Entity id = _availableEntities.front();
        _availableEntities.pop();
        ++_livingEntityCount;

        return id;
    }

    void EntityManager::destroyEntity(Entity entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        _signatures[entity].reset();

        _availableEntities.push(entity);
        --_livingEntityCount;
    }

    void EntityManager::setSignature(Entity entity, Signature signature) {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        _signatures[entity] = signature;
    }

    Signature EntityManager::getSignature(Entity entity) const {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return _signatures[entity];
    }
}