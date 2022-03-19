#include "CameraManager.h"
#include "Core.h"
#include "Keybinds.h"

namespace Plasmium {

    void CameraManager::Initialize() {
    }

    void CameraManager::ProcessEvent(const GenericEvent& event) {
        auto& resourceManager = Core::GetInstance().GetResourceManager();
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);

            const float movespeed = .25f;
            const float rotationspeed = 2.0f;
            if (inputEvent.GetKeyDown(KeybindFunction::RotateCameraLeft)) {
                currentCamera->MoveRotation(vec3(rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::RotateCameraRight)) {
                currentCamera->MoveRotation(vec3(-rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraForward)) {
                currentCamera->MovePositionRelative(vec2(0.0f, movespeed));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraBackward)) {
                currentCamera->MovePositionRelative(vec2(0.0f, -movespeed));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraLeft)) {
                currentCamera->MovePositionRelative(vec2(-movespeed, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraRight)) {
                currentCamera->MovePositionRelative(vec2(movespeed, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::ZoomIn)) {
                currentCamera->Zoom(-1.0f);
            }
            if (inputEvent.GetKeyDown(KeybindFunction::ZoomOut)) {
                currentCamera->Zoom(1.0f);
            }
        }
        if ((EventType)event.index() == EventType::MoveCamera) {
            auto& moveCameraEvent = std::get<MoveCameraEvent>(event);
            currentCamera->SetPosition(moveCameraEvent.position);
            currentCamera->SetRotation(moveCameraEvent.rotation);
        }
    }

    void CameraManager::Update(milliseconds deltaTime) {
        currentCamera->Update(deltaTime);
    }

    void CameraManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        const vec3& postionOffset,
        const vec3& rotation)
    {
        assert(creationArgs.type == ComponentType::Camera);
        // Assume there's only one
        currentCamera = cameraComponents.EmplaceObject(creationArgs.parent,
            CameraComponent(creationArgs, postionOffset, rotation));
    }

    void CameraManager::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::Camera);
        cameraComponents.DeleteObject(id);
    }
}

