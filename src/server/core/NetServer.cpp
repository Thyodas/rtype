/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** NetServer.cpp
*/

#include "server/core/NetServer.hpp"

#include <common/game/entities/EntityFactory.hpp>
#include <common/game/entities/Objects.hpp>

#include "server/entities/Player/PlayerNetwork.hpp"

namespace server {
    void NetServer::resPingServer(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, const rtype::net::Message<common::NetworkMessage>& msg)
    {
        // std::cout << "[" << client->getID() << "]: Server Ping\n";

        // Simply bounce message back to client
        messageClient(client, msg);
    }

    void NetServer::resClientConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client, rtype::net::Message<common::NetworkMessage>& msg)
    {
        common::game::netbody::ClientConnect body;
        msg >> body;

        common::game::EntityFactory factory;
        ecs::Entity playerShip = factory.createEntity(common::game::ObjectType::Model3D, body.shipName, {
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
        auto behave = engine::createBehavior<server::PlayerNetwork>(*this, playerShip, client->getID());
        engine::attachBehavior(playerShip, behave);

        std::cout << "[" << client->getID() << "]: Client Connect " << body.name << std::endl;
        reqServerCreatePlayerShip(client, playerShip);
        allServerAllyConnect(client, playerShip);
    }

    void NetServer::reqServerCreatePlayerShip(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client,
        ecs::Entity ship)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverCreatePlayerShip;

        auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(ship);

        //auto &model = engine::Engine::getInstance()->getComponent<ecs::components::Model3D>(ship);

        common::game::netbody::ServerCreatePlayerShip body = {
            .entityNetId = ship,
            .shipName = common::game::ObjectName::DualStriker, // TODO: get ship name from entity
            .pos = transform.pos,
        };

        msg << body;
        messageClient(client, msg);
    }

    void NetServer::allServerAllyConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client,
        ecs::Entity ship)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverAllyConnect;

        auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(ship);

        //auto &model = engine::Engine::getInstance()->getComponent<ecs::components::Model3D>(ship);

        common::game::netbody::ServerAllyConnect body = {
            .entityNetId = ship,
            .name = "Jean-Michel", // TODO: get name of player
            .shipName = common::game::ObjectName::DualStriker, // TODO: get ship name from entity
            .pos = transform.pos,
        };

        msg << body;


        messageAllClients(msg, client);

    }

    void NetServer::allServerUpdatePlayerShipPosition(ecs::Entity ship, Vector3 pos)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverUpdateShipPosition;

        common::game::netbody::ServerUpdateShipPosition body = {
            .entityNetId = ship,
            .pos = engine::Engine::getInstance()->getComponent<ecs::components::physics::rigidBody_t>(ship).velocity,
        };

        msg << body;

        messageAllClients(msg);
    }
}
