/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Metadata.hpp
*/

#pragma once

#include "server/entities/Enum.hpp"

namespace ecs {
    namespace components {
        namespace metadata {
            typedef struct metadata_s {
                server::entities::EntityType type;
            } metadata_t;
        }
    }
}
