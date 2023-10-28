#pragma once

#include "rezombie/colors/colors.h"
#include <core/strings/mutation.h>
#include <metamod/engine.h>

namespace rz
{
    using namespace cssdk;

    class HudMessageParams {
      public:
        float x_ = -1.0;
        float y_ = -1.0;
        Color color1_ = {};
        Color color2_ = {};
        int effect_ = 0;
        float fadeInTime_ = 0.0;
        float holdTime_ = 0.0;
        float fadeOutTime_ = 0.0;
        float fxTime_ = 0.0;
        int channel_ = 0;

      public:
        auto setX(const float value) -> HudMessageParams& {
            x_ = value;
            return *this;
        }

        auto setY(const float value) -> HudMessageParams& {
            y_ = value;
            return *this;
        }

        auto setColor1(const Color value) -> HudMessageParams& {
            color1_ = value;
            return *this;
        }

        auto setColor2(const Color value) -> HudMessageParams& {
            color2_ = value;
            return *this;
        }

        auto setEffect(const int value) -> HudMessageParams& {
            effect_ = value;
            return *this;
        }

        auto setFadeInTime(const float value) -> HudMessageParams& {
            fadeInTime_ = value;
            return *this;
        }

        auto setHoldTime(const float value) -> HudMessageParams& {
            holdTime_ = value;
            return *this;
        }

        auto setFadeOutTime(const float value) -> HudMessageParams& {
            fadeOutTime_ = value;
            return *this;
        }

        auto setFxTime(const float value) -> HudMessageParams& {
            fxTime_ = value;
            return *this;
        }

        auto setChannel(const int value) -> HudMessageParams& {
            channel_ = value;
            return *this;
        }
    };

    class LargeHudMessageParams {
      public:
        float x_ = -1.0;
        float y_ = -1.0;
        Color color_ = {};
        int effect_ = 0;
        float fadeInTime_ = 0.0;
        float holdTime_ = 0.0;
        float fadeOutTime_ = 0.0;
        float fxTime_ = 0.0;

      public:
        auto setX(const float value) -> LargeHudMessageParams& {
            x_ = value;
            return *this;
        }

        auto setY(const float value) -> LargeHudMessageParams& {
            y_ = value;
            return *this;
        }

        auto setColor(const Color value) -> LargeHudMessageParams& {
            color_ = value;
            return *this;
        }

        auto setEffect(const int value) -> LargeHudMessageParams& {
            effect_ = value;
            return *this;
        }

        auto setFadeInTime(const float value) -> LargeHudMessageParams& {
            fadeInTime_ = value;
            return *this;
        }

        auto setHoldTime(const float value) -> LargeHudMessageParams& {
            holdTime_ = value;
            return *this;
        }

        auto setFadeOutTime(const float value) -> LargeHudMessageParams& {
            fadeOutTime_ = value;
            return *this;
        }

        auto setFxTime(const float fxTime) -> LargeHudMessageParams& {
            fxTime_ = fxTime;
            return *this;
        }
    };
}
