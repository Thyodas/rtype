/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Main
*/

#include "client/core/Client.hpp"
#include "client/entities/EntityFactory.hpp"

#include "MovementBehaviour.hpp"
#include "TestBehaviour.hpp"
#include "common/network/network.hpp"

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};



class CustomClient : public rtype::net::ClientInterface<CustomMsgTypes>
{
    public:
    void PingServer()
    {
        rtype::net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;

        // Caution with this...
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        send(msg);
    }

    void MessageAll()
    {
        rtype::net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        send(msg);
    }
};

int main(int ac, char **av)
{
    CustomClient c;
    c.connect("127.0.0.1", 60000);
    c.PingServer();
    client::Client client;
    client::EntityFactory factory;
    ecs::Entity cube = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::RedFighter, {
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
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);


    auto move = engine::createBehavior<movement>();
    ecs::Entity gunBullet = factory.createEntity(client::ObjectType::Model3D, client::ObjectName::GunBullet, {
        {0, 0, 0},
        0,
        0,
        0,
        WHITE,
        false,
        WHITE,
        {0, 0, 0},
        {0.025, 0.025, 0.025}
    }, client::ObjectFormat::GLB);
    engine::attachBehavior(gunBullet, move);

    client.run();

    return 0;
}
