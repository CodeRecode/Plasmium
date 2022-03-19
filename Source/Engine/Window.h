#pragma once
#include "CoreSystem.h"
#include "FileResource.h"
#include "HashTable.h"
#include "Types.h"
#include "InputTypes.h"
#include "Keybinds.h"

#define _X86_
#include <windef.h>
#include <string>
#include <iostream>

namespace Plasmium
{
    class Window : public CoreSystem
    {
    private:
        static const uint32 KeyStateCount = 256;
        static const uint32 KeybindFunctionCount = (uint32)KeybindFunction::KeybindFunctionCount;
        HWND handle;
        uint8 winKeyStates[KeyStateCount]{};
        bool currentKeybindFunctionStates[KeybindFunctionCount]{};
        bool previousKeybindFunctionStates[KeybindFunctionCount]{};
        HashTable<InputKey, KeybindFunction> keybinds;

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
        void Update(milliseconds deltaTime) override;

        void Serialize();
        void Deserialize();

        template <typename... Args>
        static void WriteError(const char* error, Args&&... args) {
            std::cerr << "ERROR: " << error;
            (std::cerr << ... << args);
            std::cerr << std::endl;
        }
        static void CreateConsole();
    };
}