#pragma once
#include "PlasString.h"
#include "Types.h"
#include "Text2D.h"
#include "RingBuffer.h"

namespace Plasmium {
    class GameplayEventLog {
    private:
        const uint32 LogSize = 5;
        const float LineHeight = 15.0f;
        RingBuffer<Text2D> logs;

    public:
        void AddLog(StringId id);
        void ClearLog();

        const RingBuffer<Text2D>& GetLogs() const { return logs; }
    };
}