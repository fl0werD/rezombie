#pragma once

#include "rezombie/core/amxx_feature.h"
#include "rezombie/util.h"
#include <amxx/api.h>
#include <cssdk/dll/player.h>
#include <array>

namespace rz
{
    using namespace cssdk;
    using namespace amx;

    enum class ForwardReturn
    {
        Continue,
        Supercede,
        Break,
    };

    enum class RzHudParams
    {
        X,
        Y,
        Effect,
        Color1Red,
        Color1Green,
        Color1Blue,
        Color1Alpha,
        Color2Red,
        Color2Green,
        Color2Blue,
        Color2Alpha,
        FadeInTime,
        FadeOutTime,
        HoldTime,
        FxTime,
        Channel,
    };

    enum class RzDHudParams
    {
        X,
        Y,
        Effect,
        ColorRed,
        ColorGreen,
        ColorBlue,
        FadeInTime,
        FadeOutTime,
        HoldTime,
        FxTime,
    };

    class AmxxCommon : public AmxxFeature<>
    {
      public:
        AmxxCommon() : AmxxFeature("common")
        {
        }

        auto RegisterNatives() -> void override;
    };

    inline AmxxCommon amxxCommon;
}
