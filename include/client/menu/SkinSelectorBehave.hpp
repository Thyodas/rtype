/*
** EPITECH PROJECT, 2023
** client
** File description:
** SkinSelectorBehave.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/entities/Objects.hpp"
#include "common/game/entities/EntityFactory.hpp"

namespace client {
    class SkinSelectorBehave : public ecs::components::behaviour::Behaviour {
        public:
            SkinSelectorBehave(ecs::SceneID sceneID)
            {
                _sceneID = sceneID;
            }

            ecs::Entity createPlayer(common::game::ObjectName skin)
            {
                common::game::EntityFactory factory;
                ecs::Entity et = factory.createEntity(common::game::ObjectType::Model3D, skin, {
                    {0, 0, 0},
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 0, 0},
                    {1, 1, 1}
                });
                engine::addEntityToScene(et, _sceneID);
                return et;
            }

            void drawInfo()
            {
                int middle = GetScreenWidth() / 2;
                int height = GetScreenHeight() / 2;
                int size = 20;
                DrawText("Press space select your skin", middle, 30, size, WHITE);
                DrawText("Press left or right to change your skin", middle, 60, size, WHITE);
            }

            void onExit()
            {
                engine::destroyEntity(_player);
            }

            void update() override
            {
                drawInfo();
                if (_skin == common::game::ObjectName::None) {
                    _skin = _skins[0];
                    _player = createPlayer(_skin);
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    _isSkinSelected = true;
                    _selectedSkin = _skin;
                }
                if (IsKeyPressed(KEY_RIGHT)) {
                    _skin = _skins[(static_cast<int>(_skin) + 1) % 4];
                    engine::destroyEntity(_player);
                    _player = createPlayer(_skin);
                }
                if (IsKeyPressed(KEY_LEFT)) {
                    _skin = _skins[(static_cast<int>(_skin) - 1) < 0 ? 3 : (static_cast<int>(_skin) - 1)];
                    engine::destroyEntity(_player);
                    _player = createPlayer(_skin);
                }
            }

            bool isSkinSelected() const
            {
                return _isSkinSelected;
            }

            common::game::ObjectName getSkin() const
            {
                return _skin;
            }

            common::game::ObjectName getSelectedSkin() const
            {
                return _selectedSkin;
            }
        private:
            bool _isSkinSelected = false;
            common::game::ObjectName _selectedSkin;
            common::game::ObjectName _skin = common::game::ObjectName::None;
            std::vector<common::game::ObjectName> _skins = {
                common::game::ObjectName::DualStriker,
                common::game::ObjectName::MicroRecon,
                common::game::ObjectName::RedFighter,
                common::game::ObjectName::UltravioletIntruder
            };
            ecs::Entity _player;
        };
    }
