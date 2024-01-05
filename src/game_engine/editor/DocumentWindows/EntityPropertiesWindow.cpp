/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneTreeWindow.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/EntityPropertiesWindow.hpp"

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/GameEngine.hpp"
#include "../../TestBehaviour.hpp"

namespace engine::editor {

    EntityPropertiesWindow::EntityPropertiesWindow() : position_{0}, rotation_{0}, scale_{1} {
        // Constructor
    }

    EntityPropertiesWindow::~EntityPropertiesWindow() {
        // Destructor
    }

    void EntityPropertiesWindow::setup() {
        // Setup code
    }

    void EntityPropertiesWindow::shutdown() {
        // Shutdown code
    }

    void EntityPropertiesWindow::show() {
        ImVec2 pos = ImVec2(1507, 825);
        ImVec2 size = ImVec2(415, 272);
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        ImGui::Begin("Properties", &_opened, ImGuiWindowFlags_NoCollapse);

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // Position
            ImGui::Text("Position");
            ImGui::SameLine(); ImGui::DragFloat3("##Position", &position_.x, 0.1f);

            // Rotation
            ImGui::Text("Rotation");
            ImGui::SameLine(); ImGui::DragFloat3("##Rotation", &rotation_.x, 0.1f);

            // Scale
            ImGui::Text("Scale");
            ImGui::SameLine(); ImGui::DragFloat3("##Scale", &scale_.x, 0.1f);
        }

        // Add more properties for other components here...

        ImGui::End();
    }

    void EntityPropertiesWindow::update() {
        // Update code
    }

    void EntityPropertiesWindow::SetSelectedEntityTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale) {
        position_ = position;
        rotation_ = rotation;
        scale_ = scale;
    }
}
