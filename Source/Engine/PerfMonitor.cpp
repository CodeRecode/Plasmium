#include "PerfMonitor.h"

#include "Core.h"

namespace Plasmium {
    uint64 FileTimeToUint64(const FILETIME& ac_FileTime)
    {
        ULARGE_INTEGER lv_Large;

        lv_Large.LowPart = ac_FileTime.dwLowDateTime;
        lv_Large.HighPart = ac_FileTime.dwHighDateTime;

        return lv_Large.QuadPart;
    }

    void PerfMonitor::Initialize()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        processors = info.dwNumberOfProcessors;
        uint64 frequency = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&lastPerfTime);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        timeFrequency = frequency / 1000.0f; // frequency is in seconds

        processHandle = GetCurrentProcess();
        FILETIME _creationTime, _exitTime;
        GetProcessTimes(processHandle,
            &_creationTime,
            &_exitTime,
            &kernelTime,
            &userTime);

        lastFrameTime = lastPerfTime;
    }

    milliseconds PerfMonitor::FrameStart()
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&frameStartTime);
        milliseconds deltaTime = (frameStartTime - lastFrameTime) / timeFrequency;
        return deltaTime;
    }

    void PerfMonitor::FrameEnd()
    {
        ++frame;
        lastFrameTime = frameStartTime;

        milliseconds perfDeltaTime = (frameStartTime - lastPerfTime) / (timeFrequency);
        if (perfDeltaTime > 100) {
            FILETIME lastKernelTime = kernelTime, 
                lastUserTime = userTime,
                _creationTime,
                _exitTime;
            GetProcessTimes(processHandle,
                &_creationTime,
                &_exitTime,
                &kernelTime,
                &userTime);

            uint64 systemTime = FileTimeToUint64(kernelTime)
                + FileTimeToUint64(userTime)
                - FileTimeToUint64(lastKernelTime)
                - FileTimeToUint64(lastUserTime);

            milliseconds averageTime = (systemTime / 10000.0) / processors;

            Core::PostEvent(PerformanceCountersEvent(
                (double)(frame - lastPerfFrame) / (perfDeltaTime / 1000.0),
                averageTime / perfDeltaTime * 100.0));

            lastPerfTime = frameStartTime;
            lastPerfFrame = frame;
        }
    }
}