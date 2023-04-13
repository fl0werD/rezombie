#pragma once

#include "rezombie/amxmodx/common.h"
#include "rezombie/player/player.h"

namespace rz
{
    using namespace rz::player;

    enum class PlayerClassVars : int {
        Handle,
        Name,
        Team,
        HudColor,
        PlayerProps,
        PlayerModel,
        PlayerSound,
        Melee,
        Nightvision,
    };

    enum class PlayerClassForward : int {
        ChangeClassPre,
        ChangeClassPost,
        MAX_FORWARDS,
    };

    class AmxxPlayerClass : public AmxxFeature<PlayerClassForward, PlayerClassForward::MAX_FORWARDS> {
      public:
        AmxxPlayerClass() : AmxxFeature("player_class") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto ChangeClassPre(Player& player, int playerClassIndex, int attackerIndex) const -> ForwardReturn;

        auto ChangeClassPost(Player& player, int playerClassIndex, int attackerIndex) const -> void;
    };

    inline AmxxPlayerClass amxxPlayerClass;
}
