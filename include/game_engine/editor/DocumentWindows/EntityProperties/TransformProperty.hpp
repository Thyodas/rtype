/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include <raylib.h> // For Vector3 type

#include "AEntityProperty.hpp"

namespace engine::editor {
    class TransformProperty : public AEntityProperty {
        public:
            using ComponentType = ecs::components::physics::transform_t;

            explicit TransformProperty(const std::string& name);
            ~TransformProperty() override;

            void update() final;

            int show() final;
            void showEnd() final;

    };
}
