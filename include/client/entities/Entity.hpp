/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity
*/

#pragma once

#include "game_engine/ecs/components/Behavior.hpp"

#include <unordered_map>

namespace client {
    using behaviors = std::unordered_map<std::string, ecs::components::behavior::Behavior>;

    class Entity {
        public:
            Entity(ecs::Entity id);
            ~Entity();
            client::behaviors getbehaviors() const;
            void addBehavior(std::string& name, ecs::components::behavior::Behavior& behavior);
            void removeBehavior(std::string& name);
            ecs::Entity getId() const;
        private:
            client::behaviors _behaviors;
            ecs::Entity _id;
    };
}
