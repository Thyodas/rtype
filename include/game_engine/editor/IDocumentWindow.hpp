/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** IDocumentWindow.hpp
*/

#pragma once

namespace engine::editor {
    class IDocumentWindow {
        public:
            virtual ~IDocumentWindow() = default;
            virtual void setup() = 0;
            virtual void shutdown() = 0;
            virtual void show() = 0;
            virtual void update() = 0;
            [[nodiscard]] virtual bool isFocused() const = 0;
            [[nodiscard]] virtual bool isOpened() const = 0;
    };
} // namespace engine::editor
