/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Signature.hpp
*/

#pragma once

#include <bitset>
#include "Components.hpp"

namespace ecs {
    using Signature = std::bitset<components::MAX_COMPONENT_TYPE>;
}