#pragma once
#include "Event.h"

namespace Plasmium
{
    class CoreSystem
    {
    private:
    public:
        virtual void Initialize() {}
        virtual void Update() {}
        virtual void ProcessEvent(const GenericEvent& event) {}
        virtual ~CoreSystem() {}
    };
}