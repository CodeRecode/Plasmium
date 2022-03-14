#include <stdio.h>
#include <Windows.h>
#include "Core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Plasmium::Core::GetInstance().RunGame();
    return 0;
}