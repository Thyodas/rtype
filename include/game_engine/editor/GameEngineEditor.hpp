/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** GameEngineEditor.hpp
*/

#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IDocumentWindow.hpp"

namespace engine::editor {
    class GameEngineEditor {
        public:
            GameEngineEditor();
            ~GameEngineEditor();

            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             */
            void init();

            [[nodiscard]] bool isOpen() const;


            void update();
            void render();
            void destroy();

            void registerWindow(const std::string& name, std::shared_ptr<IDocumentWindow> window);
        private:
            void setupEngine();
            void setupStyle();
            void setupFonts();
            void setupDockspace();
            void drawMenuBar();

            bool _quit = false;
            bool _showDemoWindow = false;
            std::unordered_map<std::string, std::shared_ptr<IDocumentWindow>> _windows;


    };
}

