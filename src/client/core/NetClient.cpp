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
            auto behave = engine::createBehavior<client::PlayerNetwork>(*this, body.entityNetId);
            engine::attachBehavior(cube, behave);
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
            auto behave = engine::createBehavior<client::AllyNetwork>(*this, body.entityNetId);
            engine::attachBehavior(cube, behave);
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
                {0, 0, 0},
                {1, 1, 1}
            });
            auto behave = engine::createBehavior<client::EnemyNetwork>(*this);
            engine::attachBehavior(cube, behave);
        }
}