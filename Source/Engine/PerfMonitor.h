#pragma once
#include "Types.h"
#define _X86_
#include <windef.h>

namespace Plasmium {
    class PerfMonitor {
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

    public:
        void Initialize();
        milliseconds FrameStart();
        void FrameEnd();

        milliseconds GetFrameStartTime() const { return frameStartTime / timeFrequency; }
    };
}