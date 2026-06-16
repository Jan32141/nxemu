// SPDX-FileCopyrightText: Copyright 2020 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <array>
#include <string>

#include "yuzu_common/common_types.h"
#include <nxemu-module-spec/operating_system.h>

namespace InputSettings {

namespace NativeTrigger {
enum Values : int {
    LTrigger,
    RTrigger,

    NumTriggers,
};
} // namespace NativeTrigger

namespace NativeMouseButton {
enum Values {
    Left,
    Right,
    Middle,
    Forward,
    Back,

    NumMouseButtons,
};
} // namespace NativeMouseButton

namespace NativeMouseWheel {
enum Values {
    X,
    Y,

    NumMouseWheels,
};
} // namespace NativeMouseWheel

namespace NativeKeyboard {
enum Keys {
    None,

    A = 4,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    N1,
    N2,
    N3,
    N4,
    N5,
    N6,
    N7,
    N8,
    N9,
    N0,
    Return,
    Escape,
    Backspace,
    Tab,
    Space,
    Minus,
    Plus,
    OpenBracket,
    CloseBracket,
    Pipe,
    Tilde,
    Semicolon,
    Quote,
    Backquote,
    Comma,
    Period,
    Slash,
    CapsLockKey,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    PrintScreen,
    ScrollLockKey,
    Pause,
    Insert,
    Home,
    PageUp,
    Delete,
    End,
    PageDown,
    Right,
    Left,
    Down,
    Up,

    NumLockKey,
    KPSlash,
    KPAsterisk,
    KPMinus,
    KPPlus,
    KPEnter,
    KP1,
    KP2,
    KP3,
    KP4,
    KP5,
    KP6,
    KP7,
    KP8,
    KP9,
    KP0,
    KPDot,

    Key102,
    Compose,
    Power,
    KPEqual,

    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    Open,
    Help,
    Properties,
    Front,
    Stop,
    Repeat,
    Undo,
    Cut,
    Copy,
    Paste,
    Find,
    Mute,
    VolumeUp,
    VolumeDown,
    CapsLockActive,
    NumLockActive,
    ScrollLockActive,
    KPComma,

    Ro = 0x87,
    KatakanaHiragana,
    Yen,
    Henkan,
    Muhenkan,
    NumPadCommaPc98,

    HangulEnglish = 0x90,
    Hanja,
    KatakanaKey,
    HiraganaKey,
    ZenkakuHankaku,

    LeftControlKey = 0xE0,
    LeftShiftKey,
    LeftAltKey,
    LeftMetaKey,
    RightControlKey,
    RightShiftKey,
    RightAltKey,
    RightMetaKey,

    MediaPlayPause,
    MediaStopCD,
    MediaPrevious,
    MediaNext,
    MediaEject,
    MediaVolumeUp,
    MediaVolumeDown,
    MediaMute,
    MediaWebsite,
    MediaBack,
    MediaForward,
    MediaStop,
    MediaFind,
    MediaScrollUp,
    MediaScrollDown,
    MediaEdit,
    MediaSleep,
    MediaCoffee,
    MediaRefresh,
    MediaCalculator,

    NumKeyboardKeys,
};

static_assert(NumKeyboardKeys == 0xFC, "Incorrect number of keyboard keys.");

enum Modifiers {
    LeftControl,
    LeftShift,
    LeftAlt,
    LeftMeta,
    RightControl,
    RightShift,
    RightAlt,
    RightMeta,
    CapsLock,
    ScrollLock,
    NumLock,
    Katakana,
    Hiragana,

    NumKeyboardMods,
};

} // namespace NativeKeyboard

using AnalogsRaw = std::array<std::string, (size_t)NativeAnalogValues::NumAnalogs>;
using ButtonsRaw = std::array<std::string, (size_t)NativeButtonValues::NumButtons>;
using MotionsRaw = std::array<std::string, (size_t)NativeMotionValues::NumMotions>;
using RingconRaw = std::string;

enum class ControllerType {
    ProController,
    DualJoyconDetached,
    LeftJoycon,
    RightJoycon,
    Handheld,
    GameCube,
    Pokeball,
    NES,
    SNES,
    N64,
    SegaGenesis,
};

struct PlayerInput {
    bool connected;
    ControllerType controller_type;
    ButtonsRaw buttons;
    AnalogsRaw analogs;
    MotionsRaw motions;

    bool vibration_enabled;
    int vibration_strength;

    u32 body_color_left;
    u32 body_color_right;
    u32 button_color_left;
    u32 button_color_right;
    std::string profile_name;

    // This is meant to tell the Android frontend whether to use a device's built-in vibration
    // motor or a controller's vibrations.
    bool use_system_vibrator;
};

struct TouchscreenInput {
    bool enabled;
    std::string device;

    u32 finger;
    u32 diameter_x;
    u32 diameter_y;
    u32 rotation_angle;
};
} // namespace InputSettings
