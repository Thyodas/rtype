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
#include "common/utils/Math.hpp"

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
        auto &health = engine::Engine::getInstance()->getComponent<ecs::components::health::health_t>(playerShip);
        health.healthPoints = 10;

        auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(playerShip);
        metadata.type = server::entities::EntityType::PLAYER;
        metadata.skinName = body.shipName;

        auto behave = engine::createBehavior<server::PlayerNetwork>(*this, playerShip, client->getID(), _mainSceneID);
        engine::attachBehavior(playerShip, behave);
        engine::addEntityToScene(playerShip, _mainSceneID);

        std::cout << "[" << client->getID() << "]: Client Connect " << body.name << std::endl;
        reqServerCreatePlayerShip(client, playerShip);
        allServerAllyConnect(client, playerShip);
    }

    void NetServer::reqServerCreatePlayerShip(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client,
        ecs::Entity ship)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverCreatePlayerShip;

        auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(ship);
        auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(ship);

        metadata.client = client;

        common::game::netbody::ServerCreatePlayerShip body = {
            .entityNetId = ship,
            .shipName = metadata.skinName,
            .pos = common::utils::joltVectorToRayVector(transform.position),
        };

        msg << body;
        messageClient(client, msg);
    }

    void NetServer::allServerAllyConnect(std::shared_ptr<rtype::net::Connection<common::NetworkMessage>>& client,
        ecs::Entity ship)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverAllyConnect;

        auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(ship);
        auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(ship);

        //auto &model = engine::Engine::getInstance()->getComponent<ecs::components::Model3D>(ship);

        common::game::netbody::ServerAllyConnect body = {
            .entityNetId = ship,
            .name = "Jean-Michel", // TODO: get name of player
            .shipName = metadata.skinName,
            .pos = common::utils::joltVectorToRayVector(transform.position),
        };

        msg << body;


        messageAllClients(msg, client);

    }

    void NetServer::allServerUpdateShipPosition(ecs::Entity ship)
    {
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverUpdateShipPosition;

        auto &transform = engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(ship);

        common::game::netbody::ServerUpdateShipPosition body = {
            .entityNetId = ship,
            .pos = common::utils::joltVectorToRayVector(transform.position),
        };

        msg << body;

        messageAllClients(msg);
    }

    void NetServer::allServerFireBullet(ecs::Entity bullet, ecs::Entity sender)
    {
        std::cout << "allServerFireBullet" << std::endl;
        rtype::net::Message<common::NetworkMessage> msg;
        msg.header.id = common::NetworkMessage::serverFireBullet;

        common::game::netbody::ServerFireBullet body = {
            .entityNetId = bullet,
            .sender = sender,
            .pos = common::utils::joltVectorToRayVector(engine::Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(bullet).position),
            .direction = engine::Engine::getInstance()->getComponent<ecs::components::direction::direction_t>(bullet).direction,
            .speed = 0,
            .rotation = engine::Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(bullet).rotation,
        };

        msg << body;

        messageAllClients(msg);
    }
}
