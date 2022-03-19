#pragma once
#include "InputTypes.h"
#include "PlasString.h"
#include "HashTable.h"
#include "HashFunctions.h"

enum class KeybindFunction {
    Quit,
    ReloadLevel,
    RotateCameraLeft,
    RotateCameraRight,
    MoveCameraForward,
    MoveCameraBackward,
    MoveCameraLeft,
    MoveCameraRight,
    ZoomIn,
    ZoomOut,
    MovePlayerN,
    MovePlayerNW,
    MovePlayerW,
    MovePlayerSW,
    MovePlayerS,
    MovePlayerSE,
    MovePlayerE,
    MovePlayerNE,
    KeybindFunctionCount
};

template <>
inline uint32 HashFunc<InputKey>(InputKey value) {
    return (int32)value;
}

const HashTable<Plasmium::PlasString, KeybindFunction> StringToKeybindFunctionMap{
    {Plasmium::PlasString("Quit"), KeybindFunction::Quit},
    {Plasmium::PlasString("ReloadLevel"), KeybindFunction::ReloadLevel},
    {Plasmium::PlasString("RotateCameraLeft"), KeybindFunction::RotateCameraLeft},
    {Plasmium::PlasString("RotateCameraRight"), KeybindFunction::RotateCameraRight},
    {Plasmium::PlasString("MoveCameraForward"), KeybindFunction::MoveCameraForward},
    {Plasmium::PlasString("MoveCameraBackward"), KeybindFunction::MoveCameraBackward},
    {Plasmium::PlasString("MoveCameraLeft"), KeybindFunction::MoveCameraLeft},
    {Plasmium::PlasString("MoveCameraRight"), KeybindFunction::MoveCameraRight},
    {Plasmium::PlasString("ZoomIn"), KeybindFunction::ZoomIn},
    {Plasmium::PlasString("ZoomOut"), KeybindFunction::ZoomOut},
    {Plasmium::PlasString("MovePlayerN"), KeybindFunction::MovePlayerN},
    {Plasmium::PlasString("MovePlayerNW"), KeybindFunction::MovePlayerNW},
    {Plasmium::PlasString("MovePlayerW"), KeybindFunction::MovePlayerW},
    {Plasmium::PlasString("MovePlayerSW"), KeybindFunction::MovePlayerSW},
    {Plasmium::PlasString("MovePlayerS"), KeybindFunction::MovePlayerS},
    {Plasmium::PlasString("MovePlayerSE"), KeybindFunction::MovePlayerSE},
    {Plasmium::PlasString("MovePlayerE"), KeybindFunction::MovePlayerE},
    {Plasmium::PlasString("MovePlayerNE"), KeybindFunction::MovePlayerNE}
};

const HashTable<Plasmium::PlasString, InputKey> StringToInputKeyMap{
    {Plasmium::PlasString("Escape"), InputKey::Escape},
    {Plasmium::PlasString("LeftMouse"), InputKey::LeftMouse},
    {Plasmium::PlasString("RightMouse"), InputKey::RightMouse},
    {Plasmium::PlasString("LeftArrow"), InputKey::LeftArrow},
    {Plasmium::PlasString("UpArrow"), InputKey::UpArrow},
    {Plasmium::PlasString("RightArrow"), InputKey::RightArrow},
    {Plasmium::PlasString("DownArrow"), InputKey::DownArrow},
    {Plasmium::PlasString("A"), InputKey::A},
    {Plasmium::PlasString("B"), InputKey::B},
    {Plasmium::PlasString("C"), InputKey::C},
    {Plasmium::PlasString("D"), InputKey::D},
    {Plasmium::PlasString("E"), InputKey::E},
    {Plasmium::PlasString("F"), InputKey::F},
    {Plasmium::PlasString("G"), InputKey::G},
    {Plasmium::PlasString("H"), InputKey::H},
    {Plasmium::PlasString("I"), InputKey::I},
    {Plasmium::PlasString("J"), InputKey::J},
    {Plasmium::PlasString("K"), InputKey::K},
    {Plasmium::PlasString("L"), InputKey::L},
    {Plasmium::PlasString("M"), InputKey::M},
    {Plasmium::PlasString("N"), InputKey::N},
    {Plasmium::PlasString("O"), InputKey::O},
    {Plasmium::PlasString("P"), InputKey::P},
    {Plasmium::PlasString("Q"), InputKey::Q},
    {Plasmium::PlasString("R"), InputKey::R},
    {Plasmium::PlasString("S"), InputKey::S},
    {Plasmium::PlasString("T"), InputKey::T},
    {Plasmium::PlasString("U"), InputKey::U},
    {Plasmium::PlasString("V"), InputKey::V},
    {Plasmium::PlasString("W"), InputKey::W},
    {Plasmium::PlasString("X"), InputKey::X},
    {Plasmium::PlasString("Y"), InputKey::Y},
    {Plasmium::PlasString("Z"), InputKey::Z},
    {Plasmium::PlasString("F1"), InputKey::F1},
    {Plasmium::PlasString("F2"), InputKey::F2},
    {Plasmium::PlasString("F3"), InputKey::F3},
    {Plasmium::PlasString("F4"), InputKey::F4},
    {Plasmium::PlasString("F5"), InputKey::F5},
    {Plasmium::PlasString("F6"), InputKey::F6},
    {Plasmium::PlasString("F7"), InputKey::F7},
    {Plasmium::PlasString("F8"), InputKey::F8},
    {Plasmium::PlasString("F9"), InputKey::F9},
    {Plasmium::PlasString("F10"), InputKey::F10},
    {Plasmium::PlasString("F11"), InputKey::F11},
    {Plasmium::PlasString("F12"), InputKey::F12},
    {Plasmium::PlasString("MouseWheelUp"), InputKey::MouseWheelUp},
    {Plasmium::PlasString("MouseWheelDown"), InputKey::MouseWheelDown}
};