/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetworkBehaviour.hpp
*/

#pragma once

#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Network.hpp"
#include "game_engine/GameEngine.hpp"
#include "common/network/Client.hpp"

#include <type_traits>

namespace ecs::components::behaviour
{
    /*template <typename T>
    concept NetworkManagerBase = std::is_base_of<rtype::net::ClientInterface<std::any>, T>::value;


    template<NetworkManagerBase NetworkManager>*/
    template <typename NetworkManager>
    class NetworkBehaviour : public Behaviour, public std::enable_shared_from_this<NetworkBehaviour<NetworkManager>>
    {
    public:
        explicit NetworkBehaviour(NetworkManager &networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
            : _networkManager(networkManager), _entityNetId(entityNetId), _connectionId(connectionId)
        {
            /*auto &networkManager = reinterpret_cast<rtype::net::ClientInterface<std::any>&>(_networkManager);
            networkManager.*/
        }

        void setEntity(ecs::Entity entity) override
        {
            ecs::components::network::network_t network = {
                .entityNetId = _entityNetId,
                .connectionId = _connectionId,
            };
            engine::Engine::getInstance()->addComponent<ecs::components::network::network_t>(entity, network);
            Behaviour::setEntity(entity);
            onAttach(entity);

            // auto &net = _coord->getComponent<ecs::components::network::network_t>(_entity);
            // net.entityNetId = _entityNetId;
            // net.connectionId = _connectionId;
        }

        [[nodiscard]] uint32_t getNetId() const
        {
            auto &netData = _coord->getComponent<ecs::components::network::network_t>(_entity);
            std::cout << "id is " << _entity << std::endl;
            return netData.entityNetId;
        }

        [[nodiscard]] uint32_t getConnectionId() const
        {
            auto &netData = _coord->getComponent<ecs::components::network::network_t>(_entity);
            return netData.connectionId;
        }

        /**
         * @brief Called when the entity is attached to the scene
         * Callback: Function made to be overriden
         * @note You can for example register the network responses here
         */
        virtual void onAttach([[maybe_unused]] Entity entity)
        {
        };

    protected:
        // std::map<std::string, ResponseFunction> responses;
        NetworkManager &_networkManager;

    private:
        uint32_t _entityNetId;
        uint32_t _connectionId;
    };
} // namespace ecs::components::behaviour
