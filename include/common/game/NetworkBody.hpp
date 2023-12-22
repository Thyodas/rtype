/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** NetworkBody
*/

#pragma once

#include "raylib.h"
#include "client/entities/Objects.hpp"

namespace common::game::netbody {

    // ServerAccept,
    // ServerDeny,
    // ServerPing,
    // MessageAll,
    // ServerMessage,

    // clientConnect,
    // clientUpdatePlayerDirection,
    // clientPlayerFireBullet,

    // serverCreatePlayerShip,
    // serverPlayerTakeDamage,
    // serverPlayerDestroy,

    // serverAllyConnect,
    // serverAllyDisconnect,
    // serverAllyTakeDamage,
    // serverAllyDestroy,

    // serverFireBullet,
    // serverDestroyBullet,

    // serverUpdateShipVelocity,
    // serverUpdateShipPosition,

    // serverCreateEnemy,
    // serverUpdateEnemyVelocity,
    // serverEnemyTakeDamage,
    // serverDestroyEnemy,

    #pragma pack(push, 1)

    struct ClientConnect {
        std::string name;
        client::ObjectName shipName;
    };

    struct ClientUpdatePlayerDirection {
        Vector3 direction;
    };

    struct ClientPlayerFireBullet {
        Vector3 direction;
    };

    struct ServerCreatePlayerShip {
        uint32_t entityNetId;
        client::ObjectName shipName;
        Vector3 pos;
    };

    struct ServerPlayerTakeDamage {
        uint32_t damage;
    };

    struct ServerPlayerDestroy {

    };

    struct ServerAllyConnect {
        uint32_t entityNetId;
        std::string name;
        client::ObjectName shipName;
        Vector3 pos;
    };

    struct ServerAllyDisconnect {
        uint32_t entityNetId;
    };

    struct ServerAllyTakeDamage {
        uint32_t entityNetId;
        uint32_t damage;
    };

    struct ServerAllyDestroy {
        uint32_t entityNetId;
    };

    struct ServerFireBullet {
        uint32_t entityNetId;
        Vector3 pos;
        Vector3 direction;
        double speed;
    };

    struct ServerDestroyBullet {
        uint32_t entityNetId;
    };

    struct ServerUpdateShipVelocity {
        uint32_t entityNetId;
        Vector3 velocity;
    };

    struct ServerUpdateShipPosition {
        uint32_t entityNetId;
        Vector3 pos;
    };

    struct ServerCreateEnemy {
        uint32_t entityNetId;
        std::string name;
        client::ObjectName shipName;
        Vector3 pos;
    };

    struct ServerUpdateEnemyVelocity {
        uint32_t entityNetId;
        Vector3 velocity;
    };

    struct ServerEnemyTakeDamage {
        uint32_t entityNetId;
        uint32_t damage;
    };

    struct ServerDestroyEnemy {
        uint32_t entityNetId;
    };

    #pragma pack(pop)

}
