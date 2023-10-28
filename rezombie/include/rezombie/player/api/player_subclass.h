#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include "rezombie/player/player.h"

namespace rz
{
    enum class PlayerSubclassForward : int {
        ChangeSubclassPre,
        ChangeSubclassPost,
        MAX_FORWARDS,
    };

    class AmxxPlayerSubclass : public AmxxFeature<PlayerSubclassForward, PlayerSubclassForward::MAX_FORWARDS> {
      public:
        AmxxPlayerSubclass() : AmxxFeature("player_subclass") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto ChangeSubclassPre(int player, int subclassId) const -> ForwardReturn;
        auto ChangeSubclassPost(int player, int subclassId) -> void;
    };

    inline AmxxPlayerSubclass PlayerSubclassApi;
}
