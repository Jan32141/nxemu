#pragma once

#include "yuzu_common/common_types.h"

namespace AudioCore::Sink {

enum class AudioEngine : u32
{
    Auto = 0,
    Cubeb,
    Sdl2,
    Null,
    Oboe,
};

} // namespace AudioCore::Sink
