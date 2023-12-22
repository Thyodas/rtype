/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetworkBehaviour.hpp
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"

namespace ecs::components::behaviour
{
    template<typename NetworkManager>
    class NetworkBehaviour : public Behaviour {
        public:
        explicit NetworkBehaviour(NetworkManager& networkManager) : _networkManager(networkManager)
        {

        }

        /*
        using ResponseFunction = std::function<void(rtype::net::Message<>)>;

        void registerResponse(const std::string& name, ResponseFunction func) {
            responses[name] = func;
        }*/

        protected:
            //std::map<std::string, ResponseFunction> responses;
            NetworkManager& _networkManager;
    };
} // namespace ecs::components::behaviour
