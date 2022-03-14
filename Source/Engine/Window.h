#pragma once
#include "Types.h"
#define _X86_
#include <windef.h>
#include <string>
#include "FileResource.h"
#include "CoreSystem.h"

namespace Plasmium
{
    class Window : public CoreSystem
    {
    private:
        static const uint32 KeyStateCount = 256;
        HWND handle;
        uint8 winKeyStates[KeyStateCount]{};
        bool currentKeyStates[KeyStateCount]{};
        bool previousKeyStates[KeyStateCount]{};

        bool shouldQuit = false;
        bool vsyncEnabled = true;
        bool windowed = true;

        int32 windowHeight = 480;
        int32 windowWidth = 640;
        int32 windowX = 1000;
        int32 windowY = 300;

        FileResource configFile{ "Config.ini" };

        friend class FileResource;

    public:
        HWND GetHandle() const { return handle; }
        int32 GetHeight() const { return windowHeight; }
        int32 GetWidth() const { return windowWidth; }

        bool GetVsyncEnabled() const { return vsyncEnabled; }
        bool GetWindowed() const { return windowed; }

        bool ShouldQuit() const { return shouldQuit; }

        void Initialize() override;
        void Update() override;

        void Serialize();
        void Deserialize();

        static void WriteError(std::string error);
        static void WriteError(const char* error);
        static void CreateConsole();
    };
}