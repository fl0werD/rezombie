#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    enum class PlayerForward : int {
        Joining,
        Joined,
        GiveDefaultItems,
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
    };

    inline AmxxPlayer PlayerApi;
}
