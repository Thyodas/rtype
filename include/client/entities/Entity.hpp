/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"

#include <memory>
#include <unordered_map>

namespace client {
    using behaviours = std::unordered_map<std::string, ecs::components::behaviour::Behaviour*>;

    class Entity {
        public:
            Entity(ecs::Entity id);
            ~Entity();
            client::behaviours getbehaviours() const;
            void addBehaviour(std::string& name, ecs::components::behaviour::Behaviour& behaviour);
            void removeBehaviour(std::string& name);
            ecs::Entity getId() const;
        private:
            client::behaviours _behaviours;
            ecs::Entity _id;
    };
}
