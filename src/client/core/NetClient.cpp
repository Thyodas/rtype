/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** NetClient
*/

#include "client/core/NetClient.hpp"

#include "client/entities/Bullet/BulletNetwork.hpp"
#include "client/entities/Player/PlayerNetwork.hpp"
#include "client/entities/Ally/AllyNetwork.hpp"
#include "client/entities/Enemy/EnemyNetwork.hpp"
#include "game_engine/core/event/BulletShotEvent.hpp"


namespace client {
        void NetClient::resServerCreatePlayerShip(rtype::net::Message<common::NetworkMessage>& msg)
        {
            std::cout << "resServerCreatePlayerShip" << std::endl;
            common::game::netbody::ServerCreatePlayerShip body;

            msg >> body;

            common::game::EntityFactory factory;
            ecs::Entity cube = factory.createEntity(common::game::ObjectType::Model3D, body.shipName, {
                body.pos,
                0,
                0,
                0,
                WHITE,
                false,
                WHITE,
                {0, 0, 0},
                {1, 1, 1}
            });
            auto behave = engine::createBehavior<client::PlayerNetwork>(*this, body.entityNetId, _mainSceneID);
            engine::attachBehavior(cube, behave);
            engine::addEntityToScene(cube, _mainSceneID);

            std::cout << "Player created with id " << body.entityNetId << std::endl;
        }

        void NetClient::resServerAllyConnect(rtype::net::Message<common::NetworkMessage>& msg)
        {
            std::cout << "resServerAllyConnect" << std::endl;
            common::game::netbody::ServerAllyConnect body;

            msg >> body;

            common::game::EntityFactory factory;
            ecs::Entity cube = factory.createEntity(common::game::ObjectType::Model3D, body.shipName, {
                body.pos,
                0,
                0,
                0,
                WHITE,
                false,
                WHITE,
                {0, 0, 0},
                {1, 1, 1}
            });
            auto behave = engine::createBehavior<client::AllyNetwork>(*this, body.entityNetId, _mainSceneID);
            engine::attachBehavior(cube, behave);
            engine::addEntityToScene(cube, _mainSceneID);
        }

        void NetClient::resServerCreateEnemy(rtype::net::Message<common::NetworkMessage>& msg)
        {
            std::cout << "resServerCreateEnemy" << std::endl;
            common::game::netbody::ServerCreateEnemy body;

            msg >> body;

            common::game::EntityFactory factory;
            ecs::Entity cube = factory.createEntity(common::game::ObjectType::Model3D, body.shipName, {
                body.pos,
                0,
                0,
                0,
                WHITE,
                false,
                WHITE,
                body.rotation,
                {1, 1, 1}
            });
            auto behave = engine::createBehavior<client::EnemyNetwork>(*this, body.entityNetId, _mainSceneID);
            engine::attachBehavior(cube, behave);
            engine::addEntityToScene(cube, _mainSceneID);

            std::cout << "Enemy created with id " << body.entityNetId << std::endl;
        }

        void NetClient::resServerFireBullet(rtype::net::Message<common::NetworkMessage>& msg)
        {
                common::game::netbody::ServerFireBullet body;
                msg >> body;

                common::game::EntityFactory factory;
                ecs::Entity gunBullet = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::GunBullet, {
                    body.pos,
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    body.rotation,
                    {0.025, 0.025, 0.025}
                }, common::game::ObjectFormat::GLB);

                auto &direction = engine::Engine::getInstance()->getComponent<ecs::components::direction::direction_t>(gunBullet);
                direction.direction = body.direction;
                auto &rigidBody = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(gunBullet);
                rigidBody.velocity = {0, 0, static_cast<float>(body.speed)};

                auto behave = engine::createBehavior<client::BulletNetwork>(*this, body.entityNetId, _mainSceneID);
                engine::attachBehavior(gunBullet, behave);
                engine::addEntityToScene(gunBullet, _mainSceneID);
        }
}
