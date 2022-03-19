#pragma once
#include "CameraComponent.h"
#include "CoreSystem.h"
#include "Handler.h"

namespace Plasmium {
    class CameraManager : public CoreSystem {
    private:
        CameraComponent* currentCamera;

    public:
        void Initialize() override;
        void ProcessEvent(const GenericEvent& event) override;
        void Update(milliseconds deltaTime) override;

        const Camera* GetCamera() const { return currentCamera; }
    };
}