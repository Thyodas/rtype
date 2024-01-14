/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include <raylib.h> // For Vector3 type

#include "AEntityProperty.hpp"

namespace engine::editor {
    class RenderProperty : public AEntityProperty {
        public:
            using ComponentType = ecs::components::render::render_t;

            explicit RenderProperty(const std::string& name);
            ~RenderProperty() override;

            void update() final;

            int show() final;
            void showEnd() final;

    };
}
