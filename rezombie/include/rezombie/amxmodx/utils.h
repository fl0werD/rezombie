#pragma once

#include "rezombie/core/amxx_feature.h"
#include "rezombie/util.h"
#include <amxx/api.h>
#include <cssdk/dll/player.h>

namespace rz
{
    using namespace cssdk;
    using namespace amx;

    constexpr auto FORWARD_INVALID = -1;

    inline auto GetAmxColor24(Amx* amx, const cell address) -> const Color24&
    {
        const auto value = Address(amx, address);
        const Color24 color = {toUChar(value[0]), toUChar(value[1]), toUChar(value[2])};
        return color;
    }

    inline auto SetAmxColor24(Amx* amx, const cell address, const Color24& color) -> void
    {
        const auto returnColor = Address(amx, address);
        returnColor[0] = color.red;
        returnColor[1] = color.green;
        returnColor[2] = color.blue;
    }
}
