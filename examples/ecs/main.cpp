/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** main.cpp
*/

#include <memory>

#include "common/ecs/Coordinator.hpp"
#include "common/ecs/Signature.hpp"

#include "common/ecs/components/Physics.hpp"
#include "common/ecs/components/Drawable.hpp"
#include "common/ecs/components/Controls.hpp"

#include "common/ecs/systems/Controls.hpp"
#include "common/ecs/systems/Draw.hpp"
#include "common/ecs/systems/Physics.hpp"

ecs::Coordinator gCoordinator;

//Basic main example that simply draws a rectangle on the screen and allows
//the user to control it
int main()
{
    //Initialising the Coordinator
    gCoordinator.init();

    //Registering every component
    gCoordinator.registerComponent<ecs::components::physics::position_t>();
    gCoordinator.registerComponent<ecs::components::physics::velocity_t>();
    gCoordinator.registerComponent<ecs::components::controls::controllable_t>();
    gCoordinator.registerComponent<ecs::components::drawable::drawable_sfml_t>();

    //The signature for the physics system
    //The physics system handles all the entities having a position and velocity component
    ecs::Signature signaturePhysics;
    signaturePhysics.set(gCoordinator.getComponentType<ecs::components::physics::position_t>());
    signaturePhysics.set(gCoordinator.getComponentType<ecs::components::physics::velocity_t>());

    //The signature for the draw system
    //The draw system handles all the entities having a position and drawable component
    ecs::Signature signatureDraw;
    signatureDraw.set(gCoordinator.getComponentType<ecs::components::physics::position_t>());
    signatureDraw.set(gCoordinator.getComponentType<ecs::components::drawable::drawable_sfml_t>());

    //The signature for the control system
    //The control system handles all the entites having the controllable and velocity component
    ecs::Signature signatureControl;
    signatureControl.set(gCoordinator.getComponentType<ecs::components::controls::controllable_t>());
    signatureControl.set(gCoordinator.getComponentType<ecs::components::physics::velocity_t>());

    //Creating the different system
    auto physicsSystem = gCoordinator.registerSystem<ecs::system::PhysicsSystem>();
    auto drawSystem = gCoordinator.registerSystem<ecs::system::DrawSystemSFML>();
    auto controlSystem = gCoordinator.registerSystem<ecs::system::ControlSystem>();

    //Setting the signature for each of the system
    //This tells which entities those systems have to handle
    gCoordinator.setSystemSignature<ecs::system::PhysicsSystem>(signaturePhysics);
    gCoordinator.setSystemSignature<ecs::system::DrawSystemSFML>(signatureDraw);
    gCoordinator.setSystemSignature<ecs::system::ControlSystem>(signatureControl);

    //Creating the rectangle entiy and tying it to it all the components
    ecs::Entity player = gCoordinator.createEntity();
    gCoordinator.addComponent<ecs::components::physics::position_t>(player, {100, 100});
    gCoordinator.addComponent<ecs::components::physics::velocity_t>(player, {0, 0});
    //gCoordinator.addComponent<ecs::components::controls::controllable_t>(player, {false, false, false, false});
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
    sf::Texture texture;
    texture.loadFromFile("../../ressources/client/example.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    gCoordinator.addComponent<ecs::components::drawable::drawable_sfml_t>(player, {true, &window, sprite});

    sf::Clock clock;
    float accumulator = 0;
    const float timestep = 1.0f / 60.0f;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
        accumulator += clock.restart().asSeconds();
        while (accumulator >= timestep) {
            accumulator -= timestep;
            controlSystem->updateInputs();
        }
        controlSystem->updateControl();
        physicsSystem->updatePosition(5);
        drawSystem->handleDraw();
        window.display();
    }
}