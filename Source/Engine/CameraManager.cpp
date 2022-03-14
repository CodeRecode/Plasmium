#include "CameraManager.h"
#include "Core.h"
#include "InputTypes.h"

namespace Plasmium {

    void CameraManager::Initialize() {
    }

    void CameraManager::ProcessEvent(const GenericEvent& event) {
        auto& resourceManager = Core::GetInstance().GetResourceManager();
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);

            const float movespeed = .25f;
            const float rotationspeed = 2.0f;
            if (inputEvent.GetKeyDown(InputKey::F3)) {
                camera.MoveRotation(vec3(rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(InputKey::F4)) {
                camera.MoveRotation(vec3(-rotationspeed, 0.0f, 0.0f));
            }
            if (inputEvent.GetKeyDown(InputKey::UpArrow)) {
                camera.MovePositionRelative(vec2(0.0f, movespeed));
            }
            if (inputEvent.GetKeyDown(InputKey::DownArrow)) {
                camera.MovePositionRelative(vec2(0.0f, -movespeed));
            }
            if (inputEvent.GetKeyDown(InputKey::LeftArrow)) {
                camera.MovePositionRelative(vec2(-movespeed, 0.0f));
            }
            if (inputEvent.GetKeyDown(InputKey::RightArrow)) {
                camera.MovePositionRelative(vec2(movespeed, 0.0f));
            }
            if (inputEvent.mouseWheelDelta != 0.0f) {
                camera.Zoom(inputEvent.mouseWheelDelta);
            }
        }
        if ((EventType)event.index() == EventType::MoveCamera) {
            auto& moveCameraEvent = std::get<MoveCameraEvent>(event);
            camera.SetPosition(moveCameraEvent.position);
            camera.SetRotation(moveCameraEvent.rotation);
        }
    }
}

