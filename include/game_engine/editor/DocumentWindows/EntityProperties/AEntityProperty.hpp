/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** AEntityProperty.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include <raylib.h> // For Vector3 type

namespace engine::editor {
    class IEntityProperty {
        public:
            virtual ~IEntityProperty() = default;

            virtual void update() = 0;
            virtual void showEnd() = 0;
            virtual int show() = 0;
    };

    class AEntityProperty : public IEntityProperty {
        public:
            ~AEntityProperty() override;

            virtual void update() override;
            virtual void showEnd() override;
            virtual int show() override;

        protected:
            explicit AEntityProperty(const std::string& name);


            std::string _name;

            /**
             * @brief Ref to singleton SceneManagerBridge
             * Responsible of the communication between the editor and the engine
             */
            SceneManagerBridge& _sceneManagerBridge;
    };
}
