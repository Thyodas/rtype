/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#include "client/entities/EntityFactory.hpp"
#include <string>

ecs::Entity client::EntityFactory::createEntity(
    client::ObjectType type,
    client::ObjectName name,
    client::ObjectParams params,
    client::ObjectFormat format
)
{
    switch (type) {
        case client::ObjectType::Cube:
            return createCube(name, params, format);
        case client::ObjectType::Model3D:
            return createObject3D(name, params, format);
        default:
            throw std::invalid_argument("Invalid object type");
    }
}

ecs::Entity client::EntityFactory::createCube(
    client::ObjectName name,
    client::ObjectParams params,
    client::ObjectFormat format
)
{
    auto shape = engine::createCube(
        params.pos,
        params.width,
        params.height,
        params.length,
        params.color,
        params.toggleWire,
        params.wireColor
    );
    engine::rotate(shape, params.rotation);
    engine::scale(shape, params.scale);

    return shape;
}

ecs::Entity client::EntityFactory::createObject3D(
    client::ObjectName name,
    client::ObjectParams params,
    client::ObjectFormat format
)
{
    std::string objectName = this->objectNameToString(name);
    std::string objectFormat = this->objectFormatToString(format);
    // TO USE USING CMAKE (Depending on the path of the executable)
    // std::string path = "./ressources/client/Objects3D/" + objectName + "/" + objectName + objectFormat;
    // TO USE IF USING LOCAL MAKEFILE
    std::string path = "../../ressources/client/Objects3D/" + objectName + "/" + objectName + objectFormat;

    auto shape = engine::createModel3D(
        path.c_str(),
        params.pos,
        params.color
    );
    engine::rotate(shape, params.rotation);
    engine::scale(shape, params.scale);

    return shape;
}

std::string client::EntityFactory::objectNameToString(client::ObjectName name)
{
    switch (name) {
        case DualStriker:           return "DualStriker";
        case MicroRecon:            return "MicroRecon";
        case RedFighter:            return "RedFighter";
        case UltravioletIntruder:   return "UltravioletIntruder";
        case GunBullet:             return "GunBullet";
        default:                    throw std::invalid_argument("Invalid object name");
    }
}

std::string client::EntityFactory::objectFormatToString(client::ObjectFormat format)
{
    switch (format) {
        case OBJ: return ".obj";
        case GLB: return ".glb";
        default:
            throw std::invalid_argument("Invalid object format");
    }
}
