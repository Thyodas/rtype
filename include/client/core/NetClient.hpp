/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetClient.hpp
*/

#pragma once

#include "common/network/network.hpp"

namespace client {
    enum class CustomMsgTypes : uint32_t
    {
        ServerAccept,
        ServerDeny,
        ServerPing,
        MessageAll,
        ServerMessage,
    };

    class NetClient : public rtype::net::ClientInterface<CustomMsgTypes>
    {
        public:
        void reqPingServer()
        {
            rtype::net::Message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::ServerPing;

            std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

            msg << timeNow;
            send(msg);
        }

        void messageAll()
        {
            rtype::net::Message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::MessageAll;
            send(msg);
        }

        void registerResponse(CustomMsgTypes id, const std::function<void(rtype::net::Message<CustomMsgTypes>)> func)
        {
            _responses.insert(std::make_pair(id, func));
        }

        void registerResponse(std::vector<std::pair<CustomMsgTypes, std::function<void(rtype::net::Message<CustomMsgTypes>)>>> responses)
        {
            for (auto &response : responses)
                _responses.insert(response);
        }

        void dispatchResponse(rtype::net::Message<CustomMsgTypes>& msg)
        {
            const auto &[first, second] = _responses.equal_range(msg.header.id);
            for (auto it = first; it != second; ++it)
                it->second(msg);
        }

        void dispatchAllResponses()
        {
            auto &incoming = getIncoming();
            while (!incoming.empty()) {
                auto msg = incoming.pop_front().msg;
                dispatchResponse(msg);
            }
        }

        protected:
            std::unordered_multimap<CustomMsgTypes, std::function<void(rtype::net::Message<CustomMsgTypes>)>> _responses;
    };
}