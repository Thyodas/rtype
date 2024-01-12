/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"

#include "game_engine/editor/DocumentWindows/Main3DScene.hpp"

#include <loguru/loguru.hpp>

#include "game_engine/ecs/components/Physics.hpp"
#include "../../TestBehaviour.hpp"

class input;

engine::editor::Main3DScene::Main3DScene()
{
    setupTexture();
}

engine::editor::Main3DScene::~Main3DScene()
{
}

void engine::editor::Main3DScene::setup()
{
    setupWindow();
    setupScene();
}

void engine::editor::Main3DScene::shutdown()
{
    UnloadRenderTexture(_viewTexture);
}



void engine::editor::Main3DScene::show()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

    if (ImGui::Begin("3D View", &_opened, ImGuiWindowFlags_NoScrollbar))
    {
        renderToolbar();
        renderView();
        ImGui::PushID(0);
        renderGizmo();
        ImGui::PopID();







        // Draw the rest of the window contents
        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        _prevWindowSize = _currentWindowSize;
        _currentWindowSize = ImGui::GetWindowSize();


    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::update()
{
    if (!_opened)
        return;
    if (isWindowResized())
    {
        UnloadRenderTexture(_viewTexture);
        _viewTexture = LoadRenderTexture(static_cast<int>(_viewSize.x), static_cast<int>(_viewSize.y));
    }

    // limit fps to _targetFPS with clock, frameTime in ms
    auto frameTimeMs = static_cast<long long>(1.0 / _targetFPS * 1000.0);

    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();


    if (elapsedTime < frameTimeMs)
        return;

    engine::runEngineTextureMode(_viewTexture);
    lastTime = currentTime;

}

void engine::editor::Main3DScene::setupWindow()
{
    ImVec2 pos = ImVec2(118, 24);
    ImVec2 size = ImVec2(1389, 804);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    _currentWindowSize = size;
    _viewSize = size;
}

void engine::editor::Main3DScene::setupScene()
{
    ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
    ImGuizmo::Enable(true);
    setupCamera();
    loadEntities();
}

void engine::editor::Main3DScene::setupCamera()
{
    engine::camera::setPosition(Vector3{7.0f, 7.0f, 7.0f});
    engine::camera::setTarget(Vector3{0.0f, 2.0f, 0.0f});
}

void engine::editor::Main3DScene::setupTexture()
{
    _viewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

void engine::editor::Main3DScene::loadEntities()
{
    ecs::Entity cube = engine::createCube({0, 2, 0}, 4, 4, 4, RED, true);
    _selectedEntity = cube;
    //engine::setRotation(cube, {deg2rad(30), 0, 0});
    //engine::setRotation(cube, {deg2rad(-10), 0, 0});
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);
    ecs::Entity cube2 = engine::createCube({5, 1, 0}, 2, 2, 2);
}

void engine::editor::Main3DScene::handleWindowResize()
{
}

void engine::editor::Main3DScene::renderToolbar()
{
    ImVec2 buttonSize = ImVec2(40, 40); // Set the desired width and height
    float padding = 0.0f; // Set the desired padding between buttons

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));

    if (ImGui::Button("Orthographic")) {
        // Switch to orthographic camera mode
        //engine::camera::setMode(CameraMode::ORTHOGRAPHIC);
    }

    ImGui::SameLine();
    if (ImGui::Button("Perspective")) {
        // Switch to perspective camera mode
        //engine::camera::setMode(CameraMode::PERSPECTIVE);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_HAND_POINTER, buttonSize)) {
        // Set cursor to normal mode
        //engine::input::setCursorMode(CursorMode::NORMAL);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_HAND, buttonSize)) {
        // Set cursor to pan mode
        //engine::input::setCursorMode(CursorMode::PAN);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ARROWS_SPIN, buttonSize)) {
        // Set cursor to rotate mode
        //engine::input::setCursorMode(CursorMode::ROTATE);
    }

    ImGui::SameLine();
    // make the slider not too large
    ImGui::PushItemWidth(100); // Set width for slider
    if (ImGui::DragInt("Target FPS", &_targetFPS, 1, 1, 120)) {
        // Set target FPS
    }
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::renderGizmo()
{
    /*
    // Example setup for view and projection matrices
    Matrix cameraView = engine::camera::getViewMatrix();
    Matrix cameraProjection = MatrixPerspective(
        camera::getFov() * DEG2RAD,           // Field of view
        (float)GetScreenWidth() / (float)GetScreenHeight(), // Aspect ratio
        0.1f,                      // Near clipping plane
        1000.0f                    // Far clipping plane
    );

    // Convert raylib Matrix to float array expected by ImGuizmo
    auto viewMatrix = MatrixToFloat(cameraView);
    auto projectionMatrix = MatrixToFloat(cameraProjection);

    // Example object matrix setup
    Transform objectTransform; // This would be your object's transform component
    objectTransform.translation = Vector3{0, 0, 0};
    objectTransform.rotation = QuaternionIdentity();
    objectTransform.scale = Vector3{1, 1, 1};

    // Extract the translation, rotation, and scale from the objectTransform
    Vector3 position = objectTransform.translation;
    Quaternion rotationQuat = objectTransform.rotation;
    Vector3 scale = objectTransform.scale;

    // Convert the quaternion to a rotation matrix
    Matrix rotation = QuaternionToMatrix(rotationQuat);

    // Create individual matrices
    Matrix translation = MatrixTranslate(position.x, position.y, position.z);
    Matrix scaling = MatrixScale(scale.x, scale.y, scale.z);

    // Combine the matrices
    Matrix transform = MatrixMultiply(MatrixMultiply(scaling, rotation), translation);

    // ... (rest of your code for setting up ImGuizmo)

    // Manipulate the matrix with ImGuizmo
    ImGuizmo::Manipulate(viewMatrix, projectionMatrix,
                         ImGuizmo::OPERATION::TRANSLATE,
                         ImGuizmo::MODE::WORLD,
                         MatrixToFloat(transform));

    // Apply the transformed matrix back to your object
    // This is where you'd convert the float array back to your transform component
    // and update the object's transform accordingly

    ImGuizmo::DrawCubes(viewMatrix, projectionMatrix, MatrixToFloat(transform), 1);
    */



    ImGuizmo::SetID(_selectedEntity);
    // Assuming you have these functions implemented
    Matrix viewMatrix = engine::camera::getViewMatrix();
    Matrix projectionMatrix = engine::camera::getProjectionMatrix(_currentWindowSize.x / _currentWindowSize.y, 0.1f, 1000.0f);

    // Assuming you have a way to get the selected object's transform
    //Transform objectTransform = engine::entity::getTransform(_selectedEntity);

    // Convert transform to matrix
    Matrix objectMatrix = engine::entity::getTransformMatrix(_selectedEntity);

    LOG_F(INFO, "objectMatrix: %f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
          objectMatrix.m0, objectMatrix.m1, objectMatrix.m2, objectMatrix.m3,
          objectMatrix.m4, objectMatrix.m5, objectMatrix.m6, objectMatrix.m7,
          objectMatrix.m8, objectMatrix.m9, objectMatrix.m10, objectMatrix.m11,
          objectMatrix.m12, objectMatrix.m13, objectMatrix.m14, objectMatrix.m15);

    // Convert raylib matrices to float arrays for ImGuizmo
    float *viewMatrixFloats = MatrixToFloat(viewMatrix);
    float *projectionMatrixFloats = MatrixToFloat(projectionMatrix);
    float *objectMatrixFloats = MatrixToFloat(objectMatrix);

    // Set ImGuizmo context (window size, etc.)
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);

    // Manipulate the matrix with ImGuizmo
    ImGuizmo::Manipulate(viewMatrixFloats, projectionMatrixFloats,
                         ImGuizmo::OPERATION::TRANSLATE,
                         ImGuizmo::MODE::WORLD,
                         objectMatrixFloats);

    // Check if the matrix was changed
    if (ImGuizmo::IsUsing())
    {
        /*
        // Update the object's transform with the new matrix
        engine::UpdateSelectedObjectTransform(updatedMatrix);*/
        float translation[3];
        float rotation[3];
        float scale[3];
        ImGuizmo::DecomposeMatrixToComponents(objectMatrixFloats, translation, rotation, scale);
        Vector3 pos = {translation[0], translation[1], translation[2]};
        Vector3 rot = {rotation[0], rotation[1], rotation[2]};
        Vector3 sca = {scale[0], scale[1], scale[2]};
        LOG_F(WARNING, "pos: %f %f %f\nrot: %f %f %f\nsca: %f %f %f", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, sca.x, sca.y, sca.z);
        engine::entity::setTransform(_selectedEntity, pos, rot, sca);
        LOG_F(INFO, "pos: %f %f %f", pos.x, pos.y, pos.z);
    }
}

void engine::editor::Main3DScene::renderView()
{
    _viewPosition = ImGui::GetCursorScreenPos();
    _viewSize = ImGui::GetContentRegionAvail();
    rlImGuiImageRenderTexture(&_viewTexture);
}

bool engine::editor::Main3DScene::isWindowResized() const
{
    return _currentWindowSize.x != _prevWindowSize.x || _currentWindowSize.y != _prevWindowSize.y;
}
