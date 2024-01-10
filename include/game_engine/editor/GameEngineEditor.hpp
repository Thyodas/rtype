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
#include <loguru/loguru.hpp>

#include "IDocumentWindow.hpp"



namespace engine::editor {
    constexpr auto LOGURU_CALLBACK_NAME = "GEE";

    struct LogMessage {
        loguru::Verbosity verbosity;
        std::string message;
        std::string prefix;
    };

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
            void addLog(const LogMessage& message);
            [[nodiscard]] const std::vector<LogMessage>& getLogs() const;
        private:
            void setupLogs();
            void setupEngine();
            void setupStyle();
            void setupFonts();
            void setupDockspace();
            void drawMenuBar();

            static void loguruCallback(void *user_data, const loguru::Message& message);

            bool _quit = false;
            bool _showDemoWindow = false;
            std::unordered_map<std::string, std::shared_ptr<IDocumentWindow>> _windows;

            std::vector<LogMessage> _logs;


    };
}

