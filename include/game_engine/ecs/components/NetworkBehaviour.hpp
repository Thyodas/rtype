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
        }

        ~NetworkBehaviour()
        {
            unregisterResponses();
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
        }

        [[nodiscard]] uint32_t getNetId() const
        {
            auto &netData = _coord->getComponent<ecs::components::network::network_t>(_entity);
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

        using ResponseFunction = std::function<void(rtype::net::Message<common::NetworkMessage> msg)>;

        void addResponse(const std::vector<std::pair<common::NetworkMessage, ResponseFunction>>& responses)
        {
            auto responseIds = _networkManager.registerResponse(responses);
            _responses.insert(_responses.end(), responseIds.begin(), responseIds.end());
        }

        void addResponse(common::NetworkMessage messageType, const ResponseFunction& func)
        {
            _responses.push_back(_networkManager.registerResponse(messageType, func));
        }

        void unregisterResponses(void)
        {
            for (auto id : _responses)
                _networkManager.unregisterResponse(id);
        }

    protected:
        NetworkManager &_networkManager;

    private:
        uint32_t _entityNetId;
        uint32_t _connectionId;

        std::vector<int> _responses;
    };
}
