#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    enum class LongJumpForward : int {
        Give,
        State,
        Activated,
        CooldownTimer,
        MAX_FORWARDS,
    };

    class AmxxLongJump : public AmxxFeature<LongJumpForward, LongJumpForward::MAX_FORWARDS> {
      public:
        AmxxLongJump() : AmxxFeature("long_jump") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto Give(int player) const -> void;
        auto State(int player, int longJumpState) const -> void;
        auto Activated(int player) const -> void;
        auto CooldownTimer(int player, int timer) const -> void;
    };

    inline AmxxLongJump LongJumpApi;
}
