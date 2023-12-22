/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetworkMessage.hpp
*/

#pragma once

namespace common {

    enum class NetworkMessage : uint32_t {
        ServerAccept,
        ServerDeny,
        ServerPing,
        MessageAll,
        ServerMessage,

        clientUpdatePlayerVelocity,
        clientPlayerFireBullet,

        serverAllyConnection,

        serverFireBullet,
        serverDestroyBullet,

        serverCreateEnemy,
        serverUpdateEnemyVelocity,
    };

}
