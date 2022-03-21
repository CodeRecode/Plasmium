#pragma once
#include "Types.h"
#define _X86_
#include <windef.h>

namespace Plasmium {
    class Timer {
    private:
        HANDLE processHandle = nullptr;

        uint64 frame = 0;
        uint64 frameStartTime = 0;
        double timeFrequency = 0;
        uint64 processors = 0;

        uint64 lastPerfTime = 0;
        uint64 lastPerfFrame = 0;
        uint64 lastFrameTime = 0; 
        
        FILETIME kernelTime;
        FILETIME userTime;

        uint32 lastRandom;

    public:
        void Initialize();
        milliseconds FrameStart();
        void FrameEnd();

        uint32 GetNextRandom();
        milliseconds GetFrameStartTime() const { return frameStartTime / timeFrequency; }
    };
}