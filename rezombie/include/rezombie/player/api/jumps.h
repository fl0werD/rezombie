#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include "rezombie/player/player.h"

namespace rz
{
    enum class JumpsForward : int {
        Jump,
        MAX_FORWARDS,
    };

    class AmxxJumps : public AmxxFeature<JumpsForward, JumpsForward::MAX_FORWARDS> {
      public:
        AmxxJumps() : AmxxFeature("jumps") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto Jump(int player, int count) const -> void;
    };

    inline AmxxJumps JumpsApi;
}
