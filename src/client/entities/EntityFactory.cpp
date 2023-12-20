/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#include "client/entities/EntityFactory.hpp"
#include <string>

ecs::Entity client::EntityFactory::createEntity(client::ObjectType type, client::ObjectName name, client::ObjectParams params)
{
    switch (type) {
        case client::ObjectType::Cube:
            return createCube(name, params);
        case client::ObjectType::Model3D:
            return createObject3D(name, params);
        default:
            throw std::invalid_argument("Invalid object type");
    }
}

ecs::Entity client::EntityFactory::createCube(client::ObjectName name, client::ObjectParams params)
{
    return engine::createCube(
        params.pos,
        params.width,
        params.height,
        params.length,
        params.color,
        params.toggleWire,
        params.wireColor
    );
}

ecs::Entity client::EntityFactory::createObject3D(client::ObjectName name, client::ObjectParams params)
{
    std::string objectName = this->objectNameToString(name);
    // TO USE USING CMAKE (Depending on the path of the executable)
    // std::string path = "./ressources/client/Objects3D/" + objectName + "/" + objectName + ".obj";
    // TO USE IF USING LOCAL MAKEFILE
    std::string path = "../../ressources/client/Objects3D/" + objectName + "/" + objectName + ".obj";

    std::cout << "path: " << path << std::endl;

    return engine::createModel3D(
        path.c_str(),
        params.pos,
        params.color
    );
}

std::string client::EntityFactory::objectNameToString(client::ObjectName name)
{
    switch (name) {
        case DualStriker:           return "DualStriker";
        case MicroRecon:            return "MicroRecon";
        case RedFighter:            return "RedFighter";
        case UltravioletIntruder:   return "UltravioletIntruder";
        default:                    throw std::invalid_argument("Invalid object name");
    }
}
