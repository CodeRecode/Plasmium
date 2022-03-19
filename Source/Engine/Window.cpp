#include "Window.h"
#include <hidusage.h>
#include <sstream>
#include "Core.h"

namespace Plasmium
{
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        {
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    void Window::Initialize() {
        Deserialize();

        WNDCLASSEX wc;
        HINSTANCE hInstance = GetModuleHandle(NULL);

        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = "PSMWindowClass";

        if (!RegisterClassEx(&wc))
        {
            return;
        }

        const uint32 ExtraWindowWidth = 16;
        const uint32 ExtraWindowHeight = 39;

        handle = CreateWindowEx(NULL,
            wc.lpszClassName,
            "Plasmium Engine",
            WS_OVERLAPPEDWINDOW,
            windowX,
            windowY,
            windowWidth + ExtraWindowWidth,
            windowHeight + ExtraWindowHeight,
            NULL,
            NULL,
            hInstance,
            NULL);
        
        ShowWindow(handle, SW_SHOW);
        SetForegroundWindow(handle);
        SetFocus(handle);

        RAWINPUTDEVICE rid[1];
        ZeroMemory(rid, sizeof(rid));
        rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
        rid[0].dwFlags = RIDEV_INPUTSINK;
        rid[0].hwndTarget = handle;
        RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
    }

    void Window::Update(milliseconds deltaTime)
    {

        float mouseWheelDelta = 0.0f;

        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_KEYDOWN) {
                if (msg.wParam == VK_ESCAPE) {
                    shouldQuit = true;
                }
            }

            if (msg.message == WM_INPUT) {
                uint32 dwSize = sizeof(RAWINPUT);
                static BYTE lpb[sizeof(RAWINPUT)];

                GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

                RAWINPUT* raw = (RAWINPUT*)lpb;
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    float xPosRelative = (float)raw->data.mouse.lLastX;
                    float yPosRelative = (float)raw->data.mouse.lLastY;
                }
            }

            if (msg.message == WM_MOUSEWHEEL) {
                int16 zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                mouseWheelDelta = zDelta / 120.0f;
            }

            if (msg.message == WM_QUIT) {
                shouldQuit = true;
            }
        }

        bool _result = GetKeyboardState(static_cast<PBYTE>(winKeyStates));
        if (!_result) {
            assert(false);
        }

        const int32 highbit = 1 << 7;
        // Convert the mouse wheel into a binary state
        if (mouseWheelDelta < 0) {
            winKeyStates[(uint32)InputKey::MouseWheelUp] = highbit;
        }
        if (mouseWheelDelta > 0) {
            winKeyStates[(uint32)InputKey::MouseWheelDown] = highbit;
        }

        memcpy(previousKeybindFunctionStates, currentKeybindFunctionStates, KeybindFunctionCount);
        for (auto& keyBindPair : keybinds) {
            currentKeybindFunctionStates[(uint32)keyBindPair.value] =
                winKeyStates[(uint32)keyBindPair.key] & highbit;
        }

        Core::GetInstance().PostEvent(InputEvent(currentKeybindFunctionStates, previousKeybindFunctionStates));
    }

    void Window::Serialize()
    {
    }

    void Window::Deserialize()
    {
        auto input = configFile.GetInputStream();
        std::string keybindFileName;

        std::string line, name, equal, value;
        while (std::getline(input, line)) {
            std::stringstream sstream(line);
            sstream >> name >> equal;
            if (name == "vsync_enabled") {
                sstream >> vsyncEnabled;
            }
            else if (name == "windowed") {
                sstream >> windowed;
            }
            else if (name == "window_height") {
                sstream >> windowHeight;
            }
            else if (name == "window_width") {
                sstream >> windowWidth;
            }
            else if (name == "window_x") {
                sstream >> windowX;
            }
            else if (name == "window_y") {
                sstream >> windowY;
            }
            else if (name == "keybinds") {
                sstream >> keybindFileName;
            }
            else {
                Window::WriteError("Undefined config value: ", name);
            }
        }
        input.close();
        FileResource keybindFile(keybindFileName.c_str());
        input = keybindFile.GetInputStream();
        while (std::getline(input, line)) {
            std::stringstream sstream(line);
            sstream >> name >> equal >> value;
            PlasString functionName(name.c_str());
            PlasString keyName(value.c_str());

            if (!StringToKeybindFunctionMap.Contains(functionName)) {
                Window::WriteError("Undefined function name: ", functionName.Get());
                continue;
            }
            if (!StringToInputKeyMap.Contains(keyName)) {
                Window::WriteError("Undefined key name: ", keyName.Get());
                continue;
            }

            auto function = StringToKeybindFunctionMap[functionName];
            auto key = StringToInputKeyMap[keyName];

            keybinds[key] = function;
        }
        input.close();
    }

    void Window::CreateConsole()
    {
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "conout$", "w", stdout);
        freopen_s(&stream, "conout$", "w", stderr);
    }
}
