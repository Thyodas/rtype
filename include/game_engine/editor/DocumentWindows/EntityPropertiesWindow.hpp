/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** EntityPropertiesWindow.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include "editor/DocumentWindows/EntityProperties/AEntityProperty.hpp"
#include <raylib.h> // For Vector3 type

namespace engine::editor {
    class EntityPropertiesWindow : public ADocumentWindow {
        public:
            EntityPropertiesWindow();
            ~EntityPropertiesWindow() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

            void SetSelectedEntityTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

        private:
            Vector3 position_;
            Vector3 rotation_;
            Vector3 scale_;

            std::unordered_map<std::type_index, std::shared_ptr<IEntityProperty>> _entityPropertiesMap;

        // Add more properties as needed
    };
}
