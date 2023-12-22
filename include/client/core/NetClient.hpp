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
    };
}