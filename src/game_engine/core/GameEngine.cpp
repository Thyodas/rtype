/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "game_engine/GameEngine.hpp"

#include "game_engine/ecs/components/Behaviour.hpp"
#include "raymath.h"
#include "common/utils/Math.hpp"
#include "common/utils/UtilsJolt.hpp"

std::shared_ptr<ecs::Coordinator> ecs::components::behaviour::Behaviour::_coord = nullptr;
std::shared_ptr<ecs::Coordinator> ecs::system::System::_coord = nullptr;
namespace engine {

    Engine *Engine::engine = nullptr;
    std::mutex Engine::_mutex;
    void Engine::init(bool disableRender)
    {

        // Create class that filters object vs broadphase layers
        // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!

        // Create class that filters object vs object layers
        // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!

        // Now we can create the actual physics system.
        JPH::RegisterDefaultAllocator();

        // Create a factory
        JPH::Factory::sInstance = new JPH::Factory();

        // Register all Jolt physics types
        JPH::RegisterTypes();
        _joltPhysicsSystemWorld.Init(
            ecs::system::cMaxBodies,
            ecs::system::cNumBodyMutexes,
            ecs::system::cMaxBodyPairs,
            ecs::system::cMaxContactConstraints,
            _broadPhaseLayerInterface,
            _objectVsBroadphaseLayerFilter,
            _objectVsObjectLayerFilter);

        ecs::system::MyContactListener *listener = new ecs::system::MyContactListener();
        _joltPhysicsSystemWorld.SetContactListener(listener);

        _disableRender = disableRender;
        if (!_disableRender) {
            _window = std::make_shared<core::Window>();
            _window->setFPS(60);
        }
        _coordinator = std::make_shared<ecs::Coordinator>();
        _coordinator->init();
        ecs::components::behaviour::Behaviour::_coord = _coordinator;
        ecs::system::System::_coord = _coordinator;

        if (!_disableRender)
            _coordinator->registerComponent<ecs::components::render::render_t>();
        _coordinator->registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator->registerComponent<ecs::components::animations::animation_t>();
        _coordinator->registerComponent<ecs::components::network::network_t>();
        _coordinator->registerComponent<ecs::components::health::health_t>();
        _coordinator->registerComponent<ecs::components::direction::direction_t>();

        _coordinator->registerComponent<ecs::components::physics::ColliderComponent>();
        _coordinator->registerComponent<ecs::components::physics::ForceComponent>();
        _coordinator->registerComponent<ecs::components::physics::PhysicsMaterialComponent>();
        _coordinator->registerComponent<ecs::components::physics::RigidBodyComponent>();
        _coordinator->registerComponent<ecs::components::physics::TransformComponent>();
        ecs::components::input::Input input;
        _coordinator->registerSingletonComponent<ecs::components::input::Input>(input);

        ecs::Signature signatureRender;
        if (!_disableRender) {
            signatureRender.set(_coordinator->getComponentType<ecs::components::physics::TransformComponent>());
            signatureRender.set(_coordinator->getComponentType<ecs::components::render::render_t>());
        }
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator->getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        ecs::Signature signatureAnimations;
        signatureAnimations.set(_coordinator->getComponentType<ecs::components::animations::animation_t>());

        ecs::Signature signatureJoltPhysics;
        signatureJoltPhysics.set(_coordinator->getComponentType<ecs::components::physics::RigidBodyComponent>());
        signatureJoltPhysics.set(_coordinator->getComponentType<ecs::components::physics::TransformComponent>());

        if (!_disableRender) {
            _renderSystem = _coordinator->registerSystem<ecs::system::RenderSystem>();
            _coordinator->setSystemSignature<ecs::system::RenderSystem>(signatureRender);
        }

        _behaviourSystem = _coordinator->registerSystem<ecs::system::BehaviourSystem>();
        _coordinator->setSystemSignature<ecs::system::BehaviourSystem>(signatureBehaviour);

        _animationSystem = _coordinator->registerSystem<ecs::system::AnimationSystem>();
        _coordinator->setSystemSignature<ecs::system::AnimationSystem>(signatureAnimations);

        _inputSystem = _coordinator->registerSystem<ecs::system::InputSystem>();

        _joltPhysicsSystem = _coordinator->registerSystem<ecs::system::JoltPhysicsSystem>();
        _coordinator->setSystemSignature<ecs::system::JoltPhysicsSystem>(signatureJoltPhysics);
    }

    std::vector<std::pair<std::type_index, std::any>> Engine::getAllComponents(ecs::Entity entity)
    {
        return _coordinator->getAllComponents(entity);
    }

    ecs::Entity Engine::addPhysicEntity(
        ecs::components::physics::TransformComponent transf,
        ecs::components::render::render_t render,
        ecs::components::physics::RigidBodyComponent body,
        const JPH::BodyCreationSettings &creationSettings
    ) {
        JPH::EActivation activation = JPH::EActivation::Activate;
        if (body.bodyType == ecs::components::physics::BodyType::STATIC)
            activation = JPH::EActivation::DontActivate;
        ecs::Entity entity = _coordinator->createEntity();
        auto newBody = _joltPhysicsSystemWorld.GetBodyInterface().CreateBody(creationSettings);
        body.bodyID = newBody->GetID();
        _joltPhysicsSystemWorld.GetBodyInterface().AddBody(body.bodyID, activation);
        _coordinator->addComponent<ecs::components::physics::TransformComponent>(entity, transf);
        _coordinator->addComponent<ecs::components::physics::RigidBodyComponent>(entity, body);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        _entityToBody.insert({entity, body.bodyID});
        _bodyToEntity.insert({body.bodyID, entity});
        return entity;
    }

    ecs::Entity Engine::addEntity(
                ecs::components::physics::TransformComponent transf,
                ecs::components::render::render_t render
    ) {
        ecs::Entity entity = _coordinator->createEntity();
        _coordinator->addComponent<ecs::components::physics::TransformComponent>(entity, transf);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    void Engine::destroyEntity(ecs::Entity entity)
    {
        //TODO: Remove entity from the physics system if necessary
        _coordinator->destroyEntity(entity);
    }

    void Engine::run(void) {
        _inputSystem->handleInputs();
        _behaviourSystem->handleBehaviours();
        _animationSystem->handleAnimations();
        _coordinator->dispatchEvents();
        if (_disableRender)
            return;
        _window->clear(BLACK);
        BeginDrawing();
        BeginMode3D(_window->getCamera());
        _renderSystem->render();
        //DrawGrid(20, 1.0f);
        EndMode3D();
        EndDrawing();
    }

    void Engine::runTextureMode(RenderTexture& ViewTexture) {
        _behaviourSystem->handleBehaviours();
        _joltPhysicsSystem->update(_joltPhysicsSystemWorld, 1.0f / 60.0f);
        _animationSystem->handleAnimations();
        _coordinator->dispatchEvents();
        if (_disableRender)
            return;
        BeginTextureMode(ViewTexture);
        _window->clear(Color{41, 41, 41, 255});
        BeginMode3D(_window->getCamera());
        _renderSystem->render();
        DrawGrid(50, 1.0f);
        EndMode3D();
        EndTextureMode();
    }

    void Engine::update(ecs::SceneID sceneId)
    {
        if (_coordinator->isScenePaused(sceneId) || !_coordinator->isSceneActive(sceneId))
            return;
        _joltPhysicsSystemWorld.OptimizeBroadPhase();
        //activateScene(sceneId);
        _behaviourSystem->handleBehaviours();
        _joltPhysicsSystem->update(_joltPhysicsSystemWorld, 1.0f / 60.0f);
        _animationSystem->handleAnimations();
        _coordinator->dispatchEvents();
        //deactivateScene(sceneId);
    }

    void Engine::render(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        bool isSceneActive = _coordinator->isSceneActive(sceneId);
        if (!isSceneActive)
            activateScene(sceneId);
        BeginDrawing();
        //BeginTextureMode(_coordinator->getCamera(sceneId, cameraId).getRenderTexture());
        _window->clear(WHITE);
        BeginMode3D(_coordinator->getCamera(sceneId, cameraId).getCamera());
        _renderSystem->render();
        DrawGrid(10000, 1.0f);
        EndMode3D();
        //EndTextureMode();
        EndDrawing();
        if (!isSceneActive)
            deactivateScene(sceneId);
    }

    ecs::SceneID Engine::createScene()
    {
        static ecs::SceneID currentSceneId = 0;
        _coordinator->createScene(currentSceneId);
        return currentSceneId++;
    }

    void Engine::deleteScene(ecs::SceneID id)
    {
        _coordinator->deleteScene(id);
    }

    void Engine::activateScene(ecs::SceneID id)
    {
        _coordinator->activateScene(id);
    }

    void Engine::deactivateScene(ecs::SceneID id)
    {
        _coordinator->deactivateScene(id);
    }

    void Engine::pauseScene(ecs::SceneID id)
    {
        _coordinator->pauseScene(id);
    }

    bool Engine::isScenePaused(ecs::SceneID id)
    {
        return _coordinator->isScenePaused(id);
    }

    void Engine::resumeScene(ecs::SceneID id)
    {
        _coordinator->resumeScene(id);
    }

    engine::core::EngineCamera Engine::createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        engine::core::EngineCamera newCamera(_nextId, pos, target, up, mode, fov);
        _nextId++;
        return newCamera;
    }

    void Engine::attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        _coordinator->attachCamera(sceneID, camera);
    }

    void Engine::detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        _coordinator->detachCamera(sceneID, camera);
    }

    JPH::BodyID Engine::getBodyFromEntity(ecs::Entity entity) const
    {
        return _entityToBody.at(entity);
    }

    ecs::Entity Engine::getEntityFromBody(JPH::BodyID body) const
    {
        return _bodyToEntity.at(body);
    }

    void Engine::scale(ecs::Entity entity, JPH::Vec3 scale)
    {
        JPH::BodyID bodyId = _entityToBody.at(entity);
        JPH::BodyInterface &interface = _joltPhysicsSystemWorld.GetBodyInterface();
        JPH::ShapeRefC shape = interface.GetShape(bodyId);
        JPH::Ref<JPH::Shape> newShape = common::utils::createShapeWithScale(scale, shape);
        interface.SetShape(bodyId, newShape, false , JPH::EActivation::Activate);
    }

    void Engine::applyAngularImpulse(ecs::Entity entity, JPH::Vec3 impulse)
    {
        JPH::BodyID bodyId = _entityToBody.at(entity);
        JPH::BodyInterface &interface = _joltPhysicsSystemWorld.GetBodyInterface();
        interface.AddAngularImpulse(bodyId, impulse);
    }

    void Engine::applyForce(ecs::Entity entity, JPH::Vec3 force)
    {
        JPH::BodyID bodyId = _entityToBody.at(entity);
        JPH::BodyInterface &interface = _joltPhysicsSystemWorld.GetBodyInterface();
        interface.AddForce(bodyId, force);
    }

    void Engine::applyForceAndTorque(ecs::Entity entity, JPH::Vec3 force, JPH::Vec3 torque)
    {
        JPH::BodyID bodyId = _entityToBody.at(entity);
        JPH::BodyInterface &interface = _joltPhysicsSystemWorld.GetBodyInterface();
        interface.AddForceAndTorque(bodyId, force, torque);
    }

    void Engine::applyImpulse(ecs::Entity entity, JPH::Vec3 impulse)
    {
        JPH::BodyID bodyId = _entityToBody.at(entity);
        JPH::BodyInterface &interface = _joltPhysicsSystemWorld.GetBodyInterface();
        interface.AddImpulse(bodyId, impulse);
    }

    void Engine::setLinearVelocity(ecs::Entity entity, JPH::Vec3 velocity)
    {
        std::cout << "on set la velocité" << std::endl;
        _coordinator->getComponent<ecs::components::physics::RigidBodyComponent>(entity).velocity += velocity;
    }

    void initEngine(bool disableRender)
    {
        Engine::getInstance()->init(disableRender);
    }

    void runEngine()
    {
        Engine::getInstance()->run();
    }

    void runEngineTextureMode(RenderTexture& ViewTexture)
    {
        Engine::getInstance()->runTextureMode(ViewTexture);
    }

    void update(ecs::SceneID sceneId)
    {
        Engine::getInstance()->update(sceneId);
    }

    void render(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        Engine::getInstance()->render(sceneId, cameraId);
    }

    std::vector<std::pair<std::type_index, std::any>> getAllComponents(ecs::Entity entity)
    {
        return Engine::getInstance()->getAllComponents(entity);
    }

    ecs::Entity createCube(
        Vector3 pos,
        float width,
        float height,
        float length,
        Color color,
        ecs::components::physics::BodyType bodyType,
        bool toggleWire,
        Color wireColor
        )
    {
        bool subjectToGravity = true;
        JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
        if (bodyType == ecs::components::physics::BodyType::STATIC) {
            subjectToGravity = false;
            motionType = JPH::EMotionType::Static;
        }
        auto cube = std::make_shared<ecs::components::Cube>(width, height, length, toggleWire, color, wireColor);
        ecs::components::physics::TransformComponent transf(
            {pos.x, pos.y, pos.z},
            JPH::Quat::sIdentity(),
            JPH::Vec3(1, 1, 1)
        );
        ecs::components::physics::RigidBodyComponent body(1, {0, 0, 0}, subjectToGravity, bodyType);
        JPH::Ref<JPH::BoxShape> boxShape = new JPH::BoxShape(JPH::Vec3(width / 2, height / 2, length / 2), 0);
        boxShape->SetDensity(0.001);
        JPH::Ref<JPH::Shape> shapeRef = static_cast<JPH::Ref<JPH::Shape>>(boxShape);
        ecs::components::physics::ColliderComponent collider(shapeRef);
        ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, cube};
        JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(width / 2, height / 2, length / 2));
        JPH::ShapeSettings::ShapeResult boxShapeResult = boxShapeSettings.Create();

        JPH::BodyCreationSettings boxSettings(boxShape, JPH::Vec3(pos.x, pos.y, pos.z), JPH::Quat::sIdentity(), motionType, ecs::system::Layers::MOVING);
        ecs::Entity entity = Engine::getInstance()->addPhysicEntity(transf, render, body, boxSettings);
        Engine::getInstance()->addComponent<ecs::components::physics::ColliderComponent>(entity, collider);

        return entity;
    }

    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color, ecs::components::physics::BodyType bodyType)
    {
        auto model = std::make_shared<ecs::components::Model3D>(filename, color);
        bool subjectToGravity = true;
        JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
        if (bodyType == ecs::components::physics::BodyType::STATIC) {
            subjectToGravity = false;
            motionType = JPH::EMotionType::Static;
        }
        ecs::components::physics::TransformComponent transf(
            {pos.x, pos.y, pos.z},
            JPH::Quat::sIdentity(),
            JPH::Vec3(1, 1, 1)
        );
        ecs::components::physics::RigidBodyComponent body(1, {0, 0, 0}, subjectToGravity, bodyType);
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};

        JPH::Ref<JPH::BoxShape> boxShape = common::utils::convertBoundingBoxToBoxShape(GetModelBoundingBox(model->getModel()));
        boxShape->SetDensity(0.001);
        JPH::Ref<JPH::Shape> shapeRef = static_cast<JPH::Ref<JPH::Shape>>(boxShape);
        ecs::components::physics::ColliderComponent collider(shapeRef);
        JPH::BoxShapeSettings boxShapeSettings(boxShape->GetHalfExtent());
        JPH::ShapeSettings::ShapeResult boxShapeResult = boxShapeSettings.Create();
        JPH::BodyCreationSettings boxSettings(boxShape, JPH::Vec3(pos.x, pos.y, pos.z), JPH::Quat::sIdentity(), motionType, ecs::system::Layers::MOVING);

        ecs::Entity entity = Engine::getInstance()->addPhysicEntity(transf, render, body, boxSettings);
        Engine::getInstance()->addComponent<ecs::components::physics::ColliderComponent>(entity, collider);
        return entity;
    }

    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Skybox>(filename);
        ecs::components::physics::TransformComponent transf(
            {pos.x, pos.y, pos.z},
            JPH::Quat::sIdentity(),
            JPH::Vec3(1, 1, 1)
        );
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        return entity;
    }

    void destroyEntity(ecs::Entity entity)
    {
        Engine::getInstance()->destroyEntity(entity);
    }

    void setAnimation(ecs::Entity entity, const char *filename)
    {
        ecs::components::animations::animation_t anim;
        auto &draw = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        anim.animFrameCounter = 0;
        anim.model = draw.data->getModel();
        anim.animsCount = 0;
        anim.anims = LoadModelAnimations(filename, &anim.animsCount);
        Engine::getInstance()->addComponent<ecs::components::animations::animation_t>(entity, anim);
    }

    Matrix transformToMatrix(const ecs::components::physics::TransformComponent &transform) {
        //Matrix matScale = MatrixScale(1, 1, 1);
        Matrix matRotation = MatrixRotateXYZ(Vector3{0, 0, 0});
        Matrix matScale = MatrixScale(transform.scale.GetX(), transform.scale.GetY(), transform.scale.GetZ());
        //Matrix matRotation = MatrixRotateXYZ(Vector3{RAD2DEG * transform.rotation.x, RAD2DEG * transform.rotation.y, RAD2DEG * transform.rotation.z});
        Matrix matTranslation = MatrixTranslate(transform.position.GetX(), transform.position.GetY(), transform.position.GetZ());

        // First scale, then rotate, and finally translate
        Matrix transformMatrix = MatrixMultiply(matScale, matRotation); // Scale, then rotate
        transformMatrix = MatrixMultiply(transformMatrix, matTranslation); // Then translate

        return transformMatrix;
    }

    Matrix entity::getTransformMatrix(ecs::Entity entity)
    {
        const auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);

        return transformToMatrix(transform);
    }

    Transform entity::getTransform(ecs::Entity entity)
    {
        const auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);

        return Transform {
            .translation = common::utils::joltVectorToRayVector(transform.position),
            .rotation = {transform.rotation.GetX(), transform.rotation.GetY(), transform.rotation.GetZ(), transform.rotation.GetW()},
            .scale = common::utils::joltVectorToRayVector(transform.scale)
        };
    }

    void entity::setTransform(ecs::Entity entity, const Vector3 &position,
        const Vector3 &rotation, const Vector3 &scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);
        transform.position = common::utils::rayVectorToJoltVector(position);
        //transform.rotation = rotation;
        engine::rotate(entity, rotation);
        //engine::setScale(entity, scale);
    }

    void camera::setPosition(Vector3 pos)
    {
        Engine::getInstance()->getWindow()->setCameraPosition(pos);
    }

    Vector3 camera::getPosition()
    {
        return Engine::getInstance()->getWindow()->getCameraPosition();
    }

    void camera::setTarget(Vector3 pos)
    {
        Engine::getInstance()->getWindow()->setCameraTarget(pos);
    }

    Vector3 camera::getTarget()
    {
        return Engine::getInstance()->getWindow()->getCameraTarget();
    }


    void camera::setFov(float fov)
    {
        Engine::getInstance()->getWindow()->setCameraFov(fov);
    }

    float camera::getFov()
    {
        return Engine::getInstance()->getWindow()->getCameraFov();
    }

    Matrix camera::getViewMatrix()
    {
        return Engine::getInstance()->getWindow()->getCameraViewMatrix();
    }

    Matrix matrixFromFloat16(const float16& matrix)
    {
        Matrix mat;
        mat.m0 = matrix.v[0];
        mat.m1 = matrix.v[1];
        mat.m2 = matrix.v[2];
        mat.m3 = matrix.v[3];
        mat.m4 = matrix.v[4];
        mat.m5 = matrix.v[5];
        mat.m6 = matrix.v[6];
        mat.m7 = matrix.v[7];
        mat.m8 = matrix.v[8];
        mat.m9 = matrix.v[9];
        mat.m10 = matrix.v[10];
        mat.m11 = matrix.v[11];
        mat.m12 = matrix.v[12];
        mat.m13 = matrix.v[13];
        mat.m14 = matrix.v[14];
        mat.m15 = matrix.v[15];
        return mat;
    }

    void camera::setViewMatrix(Matrix matrix)
    {
        Engine::getInstance()->getWindow()->setCameraViewMatrix(matrix);
    }

    Matrix camera::getProjectionMatrix(double aspect, double nearPlane, double farPlane)
    {
        return Engine::getInstance()->getWindow()->getProjectionMatrix(aspect, nearPlane, farPlane);
    }

    void rotate(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        Matrix matTemp = MatrixRotateXYZ(rotation);
        Quaternion quatTemp = QuaternionFromMatrix(matTemp);
        transform.rotation = common::utils::rayQuatToJoltQuat(quatTemp) * transform.rotation;
    }

    void setRotation(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        Matrix matTemp = MatrixRotateXYZ(rotation);
        matTemp = MatrixTranspose(matTemp);
        Quaternion quatTemp = QuaternionFromMatrix(matTemp);
        transform.rotation = common::utils::rayQuatToJoltQuat(quatTemp);
    }

    void scale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);
        transform.scale = common::utils::rayVectorToJoltVector(scale) * transform.scale;
        Engine::getInstance()->scale(entity, transform.scale);
    }

    void setScale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::TransformComponent>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);

        // Set the scale of the transform directly to the new scale value
        transform.scale = common::utils::rayVectorToJoltVector(scale);

        // Create a scaling matrix with the new scale values
    }

    void attachBehavior(
        ecs::Entity entity,
        std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour
        )
    {
        Engine::getInstance()->addComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity, behaviour);
        behaviour->setEntity(entity);
    }

    bool isWindowOpen(void)
    {
        return Engine::getInstance()->isWindowOpen();
    }

    bool isKeyPressed(ecs::components::input::Keys key)
    {
        auto &input = Engine::getInstance()->getSingletonComponent<ecs::components::input::Input>();
        return input.keys[static_cast<size_t>(key)].keyPressed;
    }

    bool isKeyReleased(ecs::components::input::Keys key)
    {
        auto &input = Engine::getInstance()->getSingletonComponent<ecs::components::input::Input>();
        return input.keys[static_cast<size_t>(key)].keyReleased;
    }

    bool isKeyDown(ecs::components::input::Keys key)
    {
        auto &input = Engine::getInstance()->getSingletonComponent<ecs::components::input::Input>();
        return input.keys[static_cast<size_t>(key)].keyDown;
    }

    bool isKeyUp(ecs::components::input::Keys key)
    {
        auto &input = Engine::getInstance()->getSingletonComponent<ecs::components::input::Input>();
        return input.keys[static_cast<size_t>(key)].keyUp;
    }

    ecs::SceneID createScene()
    {
        return Engine::getInstance()->createScene();
    }

    void deleteScene(ecs::SceneID id)
    {
        Engine::getInstance()->deleteScene(id);
    }

    void activateScene(ecs::SceneID id)
    {
        Engine::getInstance()->activateScene(id);
    }

    void deactivateScene(ecs::SceneID id)
    {
        Engine::getInstance()->deactivateScene(id);
    }

    void pauseScene(ecs::SceneID id)
    {
        Engine::getInstance()->pauseScene(id);
    }

    void resumeScene(ecs::SceneID id)
    {
        Engine::getInstance()->resumeScene(id);
    }

    bool isScenePaused(ecs::SceneID id)
    {
        return Engine::getInstance()->isScenePaused(id);
    }

    void addEntityToScene(ecs::Entity entity, ecs::SceneID sceneID)
    {
        Engine::getInstance()->addEntityToScene(entity, sceneID);
    }

    void removeEntityFromScene(ecs::Entity entity, ecs::SceneID sceneID)
    {
        Engine::getInstance()->removeEntityFromScene(entity, sceneID);
    }

    engine::core::EngineCamera createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        return Engine::getInstance()->createCamera(pos, target, up, mode, fov);
    }

    void attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        Engine::getInstance()->attachCamera(sceneID, camera);
    }

    void detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        Engine::getInstance()->detachCamera(sceneID, camera);
    }

    JPH::BodyID physics::getBodyFromEntity(ecs::Entity entity)
    {
        return Engine::getInstance()->getBodyFromEntity(entity);
    }

    ecs::Entity physics::getEntityFromBody(JPH::BodyID body)
    {
        return Engine::getInstance()->getEntityFromBody(body);
    }

    void physics::applyAngularImpulse(ecs::Entity entity, Vector3 impulse)
    {
        JPH::Vec3 joltImpulse = common::utils::rayVectorToJoltVector(impulse);
        Engine::getInstance()->applyAngularImpulse(entity, joltImpulse);
    }

    void physics::applyForce(ecs::Entity entity, Vector3 force)
    {
        JPH::Vec3 joltForce = common::utils::rayVectorToJoltVector(force);
        Engine::getInstance()->applyForce(entity, joltForce);
    }

    void physics::applyForceAndTorque(ecs::Entity entity, Vector3 force, Vector3 torque)
    {
        JPH::Vec3 joltForce = common::utils::rayVectorToJoltVector(force);
        JPH::Vec3 joltTorque = common::utils::rayVectorToJoltVector(torque);
        Engine::getInstance()->applyForceAndTorque(entity, joltForce, joltTorque);
    }

    void physics::applyImpulse(ecs::Entity entity, Vector3 impulse)
    {
        JPH::Vec3 joltImpulse = common::utils::rayVectorToJoltVector(impulse);
        Engine::getInstance()->applyImpulse(entity, joltImpulse);
    }

    void physics::setLinearVelocity(ecs::Entity entity, Vector3 velocity)
    {
        JPH::Vec3 joltVelocity = common::utils::rayVectorToJoltVector(velocity);
        Engine::getInstance()->setLinearVelocity(entity, joltVelocity);
    }
}
