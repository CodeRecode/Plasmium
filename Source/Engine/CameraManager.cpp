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
                camera.MoveRotation(vec3(rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::RotateCameraRight)) {
                camera.MoveRotation(vec3(-rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraForward)) {
                camera.MovePositionRelative(vec2(0.0f, movespeed));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraBackward)) {
                camera.MovePositionRelative(vec2(0.0f, -movespeed));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraLeft)) {
                camera.MovePositionRelative(vec2(-movespeed, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::MoveCameraRight)) {
                camera.MovePositionRelative(vec2(movespeed, 0.0f));
            }
            if (inputEvent.GetKeyDown(KeybindFunction::ZoomIn)) {
                camera.Zoom(-1.0f);
            }
            if (inputEvent.GetKeyDown(KeybindFunction::ZoomOut)) {
                camera.Zoom(1.0f);
            }
        }
        if ((EventType)event.index() == EventType::MoveCamera) {
            auto& moveCameraEvent = std::get<MoveCameraEvent>(event);
            camera.SetPosition(moveCameraEvent.position);
            camera.SetRotation(moveCameraEvent.rotation);
        }
    }
}

