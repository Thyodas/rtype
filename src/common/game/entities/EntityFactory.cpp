/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#include "common/game/entities/EntityFactory.hpp"
#include <string>

namespace common::game {
    ecs::Entity EntityFactory::createEntity(
        ObjectType type,
        ObjectName name,
        ObjectParams params,
        ObjectFormat format
    )
    {
        switch (type) {
            case ObjectType::Cube:
                return createCube(name, params, format);
            case ObjectType::Model3D:
                return createObject3D(name, params, format);
            default:
                throw std::invalid_argument("Invalid object type");
        }
    }

    ecs::Entity EntityFactory::createCube(
        ObjectName name,
        ObjectParams params,
        ObjectFormat format
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

    ecs::Entity EntityFactory::createObject3D(
        ObjectName name,
        ObjectParams params,
        ObjectFormat format
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

    std::string EntityFactory::objectNameToString(ObjectName name)
    {
        switch (name) {
            case ObjectName::DualStriker:           return "DualStriker";
            case ObjectName::MicroRecon:            return "MicroRecon";
            case ObjectName::RedFighter:            return "RedFighter";
            case ObjectName::UltravioletIntruder:   return "UltravioletIntruder";
            case ObjectName::GunBullet:             return "GunBullet";
            case ObjectName::Transtellar:           return "Transtellar";
            default:                    throw std::invalid_argument("Invalid object name");
        }
    }

    std::string EntityFactory::objectFormatToString(ObjectFormat format)
    {
        switch (format) {
            case OBJ: return ".obj";
            case GLB: return ".glb";
            default:
                throw std::invalid_argument("Invalid object format");
        }
    }
}
