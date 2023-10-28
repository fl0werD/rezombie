#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include "rezombie/player/player.h"
#include "rezombie/player/long_jump_vars.h"

namespace rz
{
    enum class PlayerForward : int {
        Joining,
        Joined,
        GiveDefaultItems,
        LongJumpState,
        LongJumpActivated,
        MAX_FORWARDS,
    };

    class AmxxPlayer : public AmxxFeature<PlayerForward, PlayerForward::MAX_FORWARDS> {
      public:
        AmxxPlayer() : AmxxFeature("player") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto Joining(int player) const -> void;
        auto Joined(int player) const -> void;
        auto GiveDefaultItems(int player, int classId) const -> ForwardReturn;

        auto LongJumpState(int player, int longJumpState) const -> void;
        auto LongJumpActivated(int player) const -> void;
    };

    inline AmxxPlayer PlayerApi;
}
