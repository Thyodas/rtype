/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneTreeWindow.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/SceneTreeWindow.hpp"

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/GameEngine.hpp"
#include "../../TestBehaviour.hpp"

namespace engine::editor {

    SceneTreeWindow::SceneTreeWindow() {
        // Initialize the scene with some random objects
        root_.name = "Root";
        root_.children = {
            {ICON_FA_CAMERA " Camera"},
            {ICON_FA_LIGHTBULB " Lights", {
                {ICON_FA_SUN "Directional Light"},
                {ICON_FA_BOLT "Spot Light"}
            }},
            {"Static Meshes", {{"Cube"}, {"Sphere"}, {"Plane"}}},
            {"Dynamic Objects", {{"Player"}, {"Enemy #1"}, {"Enemy #2"}}},
            // ... add more as needed
        };
    }

    SceneTreeWindow::~SceneTreeWindow() {
        // Destructor
    }

    void SceneTreeWindow::setup() {
        // setup code
    }

    void SceneTreeWindow::shutdown() {
        // shutdown code
    }

    void SceneTreeWindow::ShowNode(SceneObject& object) {
        // Flags for the tree nodes
        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool leaf = object.children.empty();
        if (leaf) {
            base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        }

        // Node
        bool node_open = ImGui::TreeNodeEx(object.name.c_str(), base_flags);
        if (ImGui::IsItemClicked()) {
            // Handle the selection logic here
        }

        // Right-click context menu
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                // Rename logic here
            }
            if (ImGui::MenuItem("Delete")) {
                // Delete logic here
            }
            if (ImGui::MenuItem("Add child")) {
                // Add child logic here
            }
            ImGui::EndPopup();
        }

        if (node_open && !leaf) {
            for (auto& child : object.children) {
                ShowNode(child);
            }
            ImGui::TreePop();
        }
    }

    void SceneTreeWindow::show() {
        // Set the window to appear on the right and fill the height of the screen
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene Tree", &_opened, ImGuiWindowFlags_NoCollapse);
        ShowNode(root_);
        ImGui::End();
    }

    void SceneTreeWindow::update() {
        // update code
        if (_opened) {
            show();
        }
    }
}
