#pragma once
#include "CoreSystem.h"

namespace Plasmium {
    class CameraManager : public CoreSystem {
    private:
        Camera camera;
    public:
        void Initialize() override;
        void ProcessEvent(const GenericEvent& event) override;

        const Camera& GetCamera() const { return camera; }
    };
}