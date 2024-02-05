#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    enum class PlayerClassForward : int {
        ChangeClassPre,
        ChangeClassPost,
        MAX_FORWARDS,
    };

    class AmxxPlayerClass : public AmxxFeature<PlayerClassForward, PlayerClassForward::MAX_FORWARDS> {
      public:
        AmxxPlayerClass() : AmxxFeature("player_class") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto ChangeClassPre(int player, int classId, int attacker) const -> ForwardReturn;
        auto ChangeClassPost(int player, int classId, int attacker) const -> void;
    };

    inline AmxxPlayerClass PlayerClassApi;
}
