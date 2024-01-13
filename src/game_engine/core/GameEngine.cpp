/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "game_engine/GameEngine.hpp"

#include "game_engine/ecs/components/Behaviour.hpp"
#include "raymath.h"

std::shared_ptr<ecs::Coordinator> ecs::components::behaviour::Behaviour::_coord = nullptr;
std::shared_ptr<ecs::Coordinator> ecs::system::System::_coord = nullptr;
namespace engine {

    Engine *Engine::engine = nullptr;
    std::mutex Engine::_mutex;
    void Engine::init(bool disableRender)
    {
        _disableRender = disableRender;
        if (!_disableRender) {
            _window = std::make_shared<core::Window>();
            _window->setFPS(60);
        }
        _coordinator = std::make_shared<ecs::Coordinator>();
        _coordinator->init();
        _collisionResponseSystem = std::make_shared<ecs::system::CollisionResponse>(*_coordinator);
        ecs::components::behaviour::Behaviour::_coord = _coordinator;
        ecs::system::System::_coord = _coordinator;

        _coordinator->registerComponent<ecs::components::physics::transform_t>();
        if (!_disableRender)
            _coordinator->registerComponent<ecs::components::render::render_t>();
        _coordinator->registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator->registerComponent<ecs::components::physics::collider_t>();
        _coordinator->registerComponent<ecs::components::physics::rigidBody_t>();
        _coordinator->registerComponent<ecs::components::animations::animation_t>();
        _coordinator->registerComponent<ecs::components::network::network_t>();
        _coordinator->registerComponent<ecs::components::health::health_t>();
        _coordinator->registerComponent<ecs::components::direction::direction_t>();
        ecs::components::input::Input input;
        _coordinator->registerSingletonComponent<ecs::components::input::Input>(input);

        ecs::Signature signaturePhysics;
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::rigidBody_t>());
        ecs::Signature signatureRender;
        if (!_disableRender) {
            signatureRender.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
            signatureRender.set(_coordinator->getComponentType<ecs::components::render::render_t>());
        }
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator->getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        ecs::Signature signatureCollider;
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::collider_t>());
        ecs::Signature signatureAnimations;
        signatureAnimations.set(_coordinator->getComponentType<ecs::components::animations::animation_t>());

        _physicSystem = _coordinator->registerSystem<ecs::system::PhysicsSystem>();
        _coordinator->setSystemSignature<ecs::system::PhysicsSystem>(signaturePhysics);

        if (!_disableRender) {
            _renderSystem = _coordinator->registerSystem<ecs::system::RenderSystem>();
            _coordinator->setSystemSignature<ecs::system::RenderSystem>(signatureRender);
        }

        _behaviourSystem = _coordinator->registerSystem<ecs::system::BehaviourSystem>();
        _coordinator->setSystemSignature<ecs::system::BehaviourSystem>(signatureBehaviour);

        _collisionDetectionSystem = _coordinator->registerSystem<ecs::system::ColisionDetectionSystem>();
        _coordinator->setSystemSignature<ecs::system::ColisionDetectionSystem>(signatureCollider);

        _animationSystem = _coordinator->registerSystem<ecs::system::AnimationSystem>();
        _coordinator->setSystemSignature<ecs::system::AnimationSystem>(signatureAnimations);

        _inputSystem = _coordinator->registerSystem<ecs::system::InputSystem>();
    }

    std::vector<std::pair<std::type_index, std::any>> Engine::getAllComponents(ecs::Entity entity)
    {
        return _coordinator->getAllComponents(entity);
    }

    ecs::Entity Engine::addEntity(ecs::components::physics::transform_t transf, ecs::components::render::render_t render) {
        ecs::Entity entity = _coordinator->createEntity();
        _coordinator->addComponent<ecs::components::physics::transform_t>(entity, transf);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    void Engine::destroyEntity(ecs::Entity entity)
    {
        _coordinator->destroyEntity(entity);
    }

    void Engine::run(void) {
        _inputSystem->handleInputs();
        _behaviourSystem->handleBehaviours();
        _physicSystem->updatePosition();
        _animationSystem->handleAnimations();
        _collisionDetectionSystem->detectCollision();
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
        _physicSystem->updatePosition();
        _animationSystem->handleAnimations();
        _collisionDetectionSystem->detectCollision();
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
        //activateScene(sceneId);
        _behaviourSystem->handleBehaviours();
        _physicSystem->updatePosition();
        _animationSystem->handleAnimations();
        _collisionDetectionSystem->detectCollision();
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
        bool toggleWire,
        Color wireColor
        )
    {
        auto cube = std::make_shared<ecs::components::Cube>(width, height, length, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {pos, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, cube};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::CUBE,
            ecs::components::physics::CollisionType::COLLIDE,
            cube,
            GetModelBoundingBox(cube->getModel()),
            {0},
            MatrixIdentity(),
            MatrixIdentity(),
            MatrixIdentity()};
        Matrix matTranslate = MatrixTranslate(pos.x, pos.y, pos.z);
        collider.matTranslate = MatrixMultiply(collider.matTranslate, matTranslate);
        ecs::components::health::health_t health = {0};
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        return entity;
    }

    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Model3D>(filename, color);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::components::health::health_t health = {0};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        Engine::getInstance()->addComponent<ecs::components::direction::direction_t>(entity, {0, 0, 0});
        return entity;
    }

    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Skybox>(filename);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
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

    Matrix transformToMatrix(const ecs::components::physics::transform_t &transform) {
        //Matrix matScale = MatrixScale(1, 1, 1);
        Matrix matRotation = MatrixRotateXYZ(Vector3{0, 0, 0});
        Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
        //Matrix matRotation = MatrixRotateXYZ(Vector3{RAD2DEG * transform.rotation.x, RAD2DEG * transform.rotation.y, RAD2DEG * transform.rotation.z});
        Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);

        // First scale, then rotate, and finally translate
        Matrix transformMatrix = MatrixMultiply(matScale, matRotation); // Scale, then rotate
        transformMatrix = MatrixMultiply(transformMatrix, matTranslation); // Then translate

        return transformMatrix;
    }

    Matrix entity::getTransformMatrix(ecs::Entity entity)
    {
        const auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);

        return transformToMatrix(transform);
    }

    Transform entity::getTransform(ecs::Entity entity)
    {
        const auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);

        return Transform {
            .translation = transform.pos,
            .rotation = {transform.rotation.x, transform.rotation.y, transform.rotation.z},
            .scale = transform.scale
        };
    }

    void entity::setTransform(ecs::Entity entity, const Vector3 &position,
        const Vector3 &rotation, const Vector3 &scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        transform.pos = position;
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
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        transform.rotation = Vector3Add(transform.rotation, rotation);
        Matrix matTemp = MatrixRotateXYZ(rotation);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setRotation(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        transform.rotation = Vector3Add(transform.rotation, rotation);
        Matrix matTemp = MatrixRotateXYZ(transform.rotation);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void scale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        transform.scale = Vector3Add(transform.scale, scale);
        Matrix matTemp = MatrixScale(scale.x, scale.y, scale.z);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matScale = MatrixMultiply(collider.matScale, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setScale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);

        // Set the scale of the transform directly to the new scale value
        transform.scale = scale;

        // Create a scaling matrix with the new scale values
        Matrix matTemp = MatrixScale(scale.x, scale.y, scale.z);

        // Apply the scaling matrix to the transform of the render data and the collider
        render.data->getModel().transform = matTemp;
        collider.matScale = matTemp;

        // Update the collider's global vertices
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
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
}
