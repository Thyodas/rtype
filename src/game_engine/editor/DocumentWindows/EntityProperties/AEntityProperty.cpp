/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** AEntityProperty.cpp
*/

#include <imgui.h>

#include "game_engine/editor/DocumentWindows/EntityProperties/AEntityProperty.hpp"

namespace engine::editor {

    AEntityProperty::AEntityProperty(const std::string &name)
        : _name{name},
        _sceneManagerBridge(SceneManagerBridge::getInstance(engine::getSceneManager()))
    {

    }

    AEntityProperty::~AEntityProperty() = default;

    void AEntityProperty::update()
    {

    }

    int AEntityProperty::show() {
        return ImGui::CollapsingHeader(_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    }

    void AEntityProperty::showEnd()
    {
        //ImGui::End();
    }
}
