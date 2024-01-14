/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Metadata.hpp
*/

#pragma once

#include "server/entities/Enum.hpp"
#include "common/network/Connection.hpp"
#include "common/game/NetworkMessage.hpp"
#include "common/game/entities/Objects.hpp"

namespace ecs {
    namespace components {
        namespace metadata {
            typedef struct metadata_s {
                server::entities::EntityType type;
                std::shared_ptr<rtype::net::Connection<common::NetworkMessage>> client;
                common::game::ObjectName skinName;
            } metadata_t;
        }
    }
}
