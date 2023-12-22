/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetworkBehaviour.hpp
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"
#include "common/network/Client.hpp"

#include <type_traits>


namespace ecs::components::behaviour
{
    /*template <typename T>
    concept NetworkManagerBase = std::is_base_of<rtype::net::ClientInterface<std::any>, T>::value;


    template<NetworkManagerBase NetworkManager>*/
    template<typename NetworkManager>
    class NetworkBehaviour : public Behaviour {
        public:
        explicit NetworkBehaviour(NetworkManager& networkManager) : _networkManager(networkManager)
        {
            /*auto &networkManager = reinterpret_cast<rtype::net::ClientInterface<std::any>&>(_networkManager);
            networkManager.*/
        }

        /*template<typename TEvent>
        using ResponseFunction = std::function<void(TEvent &event)>;

        template<typename TEvent>
        void registerResponse(ResponseFunction<TEvent>)
        {

        }*/

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
