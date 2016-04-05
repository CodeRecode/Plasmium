#include <stdio.h>
#include <Windows.h>
#include "Core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    /*AllocConsole();
    FILE *stream;
    freopen_s(&stream, "conout$", "w", stdout);
    freopen_s(&stream, "conout$", "w", stderr);*/

    Plasmium::Core core;
    core.StartGame();
    core.UpdateGame();
    core.EndGame();

    return 0;
}