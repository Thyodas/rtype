/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ADocumentWindow.hpp
*/

#pragma once

#include "IDocumentWindow.hpp"

namespace engine::editor {
    class ADocumentWindow : public IDocumentWindow {
        public:
            ADocumentWindow() = default;
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return _focused; }
            [[nodiscard]] bool isOpened() const override { return _opened; }
        protected:
            bool _opened = true;
            bool _focused = false;
    };
} // namespace engine::editor
