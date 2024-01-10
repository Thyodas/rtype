/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ConsoleWindow.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include <string>
#include <vector>
#include <deque>

// Forward declaration
namespace engine::editor {
    class GameEngineEditor;
}

namespace engine::editor {

    class ConsoleWindow : public ADocumentWindow {
        public:
            explicit ConsoleWindow(const GameEngineEditor& editor)
                : _editor(editor)
            {

            }

            ~ConsoleWindow() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

            void addLog(const char* fmt, ...);
            void executeCommand(const char* command_line);

        private:
            char inputBuf[256] = {};
            std::deque<std::string> items;
            bool scrollToBottom = true;
            std::vector<std::string> commands; // History of executed commands.

            // Command and variables handling (optional)
            // std::map<std::string, std::function<void(const std::string&)>> commandsMap;

            const GameEngineEditor& _editor;

            void clearLog();
    };

}
