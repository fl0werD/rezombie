#pragma once

#include <core/strings/mutation.h>
#include <metamod/engine.h>

namespace message {
    using namespace cssdk;

    class HudMessageParams {
      public:
        float x_ = -1.0;
        float y_ = -1.0;
        int effect_ = 0;
        ColorVec color1_ = {0, 0, 0, 0};
        ColorVec color2_ = {0, 0, 0, 0};
        float fadeInTime_ = 0.0;
        float fadeOutTime_ = 0.0;
        float holdTime_ = 0.0;
        float fxTime_ = 0.0;
        int channel_ = 0;

      public:
        HudMessageParams& setX(const float value) {
            x_ = value;
            return *this;
        }

        HudMessageParams& setY(const float value) {
            y_ = value;
            return *this;
        }

        HudMessageParams& setEffect(const int value) {
            effect_ = value;
            return *this;
        }

        HudMessageParams& setColor1(const ColorVec value) {
            color1_ = value;
            return *this;
        }

        HudMessageParams& setColor2(const ColorVec value) {
            color2_ = value;
            return *this;
        }

        HudMessageParams& setFadeInTime(const float value) {
            fadeInTime_ = value;
            return *this;
        }

        HudMessageParams& setFadeOutTime(const float value) {
            fadeOutTime_ = value;
            return *this;
        }

        HudMessageParams& setHoldTime(const float value) {
            holdTime_ = value;
            return *this;
        }

        HudMessageParams& setFxTime(const float value) {
            fxTime_ = value;
            return *this;
        }

        HudMessageParams& setChannel(const int value) {
            channel_ = value;
            return *this;
        }
    };

    class DHudMessageParams {
      public:
        float x_ = -1.0;
        float y_ = -1.0;
        int effect_ = 0;
        ColorVec color_ = {0, 0, 0, 0};
        float fadeInTime_ = 0.0;
        float fadeOutTime_ = 0.0;
        float holdTime_ = 0.0;
        float fxTime_ = 0.0;

      public:
        DHudMessageParams& setX(const float value) {
            x_ = value;
            return *this;
        }

        DHudMessageParams& setY(const float value) {
            y_ = value;
            return *this;
        }

        DHudMessageParams& setEffect(const int value) {
            effect_ = value;
            return *this;
        }

        DHudMessageParams& setColor(const ColorVec value) {
            color_ = value;
            return *this;
        }

        DHudMessageParams& setFadeInTime(const float value) {
            fadeInTime_ = value;
            return *this;
        }

        DHudMessageParams& setFadeOutTime(const float value) {
            fadeOutTime_ = value;
            return *this;
        }

        DHudMessageParams& setHoldTime(const float value) {
            holdTime_ = value;
            return *this;
        }

        DHudMessageParams& setFxTime(const float fxTime) {
            fxTime_ = fxTime;
            return *this;
        }
    };
}
