#pragma once

#include "rezombie/amxmodx/common.h"
#include "rezombie/player/player.h"
#include <array>

namespace rz {
    using namespace rz::player;

    enum class RzPlayerVars : int {
        Team,
        Class,
        Subclass,
        ExtraJumps,
    };

    enum class PlayerForward : int {
        GiveDefaultItems,
        ChooseDefaultSubclass,
        LongJumpState,
        LongJumpActivated,
        MAX_PLAYER_FORWARDS,
    };

    class AmxxPlayer : public AmxxFeature<PlayerForward, PlayerForward::MAX_PLAYER_FORWARDS> {
      public:
        AmxxPlayer() : AmxxFeature("player") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto PlayerGiveDefaultItems(Player& player) const -> ForwardReturn;

        auto ChooseDefaultSubclass(Player& player) const -> void;

        auto LongJumpState(Player& player) const -> void;

        auto LongJumpActivated(Player& player) const -> void;
    };

    inline AmxxPlayer amxxPlayer;
}
