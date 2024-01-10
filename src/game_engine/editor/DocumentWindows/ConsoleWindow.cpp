/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ConsoleWindow.cpp
*/

#include "editor/DocumentWindows/ConsoleWindow.hpp"
#include <imgui.h>
#include <editor/GameEngineEditor.hpp>

namespace engine::editor {

    static constexpr const char* verbosityToString(loguru::Verbosity level) {
        switch (level) {
            case loguru::Verbosity_FATAL:   return "FATAL";
            case loguru::Verbosity_ERROR:   return "ERROR";
            case loguru::Verbosity_WARNING: return "WARNING";
            case loguru::Verbosity_INFO:    return "INFO";
            case loguru::Verbosity_INVALID:   return "INVALID";
            // Add more cases if there are other verbosity levels you use.
            default:                        return "UNKNOWN";
        }
    }

    static constexpr ImVec4 getVerbosityColor(loguru::Verbosity level)
    {
        ImVec4 color;

        switch (level) {
            case loguru::Verbosity_FATAL:   color = ImVec4(1, 0, 0, 1); break; // Red
            case loguru::Verbosity_ERROR:   color = ImVec4(1, 0.6f, 0, 1); break; // Orange
            case loguru::Verbosity_WARNING: color = ImVec4(1, 1, 0, 1); break; // Yellow
            case loguru::Verbosity_INFO:    color = ImVec4(0, 1, 0, 1); break; // Green
            case loguru::Verbosity_MAX:     color = ImVec4(0, 0.5f, 1, 1); break; // Blue
            default:                        color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    ConsoleWindow::~ConsoleWindow() {
        clearLog();
    }

    void ConsoleWindow::setup() {
        // setup code (if any)
    }

    void ConsoleWindow::shutdown() {
        // shutdown code (if any)
    }

    void ConsoleWindow::clearLog() {
        items.clear();
    }

    void ConsoleWindow::addLog(const char* fmt, ...) {
        // Store the new log line
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        items.push_back(std::string(buf));

        // Scroll to bottom
        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char* command_line) {
        // Add to history and execute command
        commands.push_back(std::string(command_line));

        // Execute logic here (expand as needed)
        addLog("# %s\n", command_line);
    }

    void ConsoleWindow::show() {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console", &_opened, ImGuiWindowFlags_NoCollapse);

        // Reserve enough space for input box
        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        // Display logs
        for (auto& item : _editor.getLogs()) {
            ImGui::TextColored(getVerbosityColor(item.verbosity),
                "[%s] %s", verbosityToString(item.verbosity),
                item.message.c_str());
        }

        // Scroll to bottom if required
        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();

        // Command line
        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            executeCommand(inputBuf);
            strcpy(inputBuf, "");
        }

        ImGui::End();
    }

    void ConsoleWindow::update() {
        // Update logic (if any)
    }

}
