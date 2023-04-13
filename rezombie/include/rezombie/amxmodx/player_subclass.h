#pragma once

#include "rezombie/amxmodx/common.h"
#include "rezombie/amxmodx/utils.h"
#include "rezombie/player/player.h"
#include <array>

namespace rz
{
    using namespace rz::player;

    enum class PlayerSubclassVars : int {
        Handle,
        Name,
        Class,
        Description,
        HudColor,
        PlayerProps,
        PlayerModel,
        PlayerSound,
        Melee,
        Nightvision,
    };

    enum class PlayerSubclassForward : int {
        ChangeSubclassPre,
        ChangeSubclassPost,
        MAX_FORWARDS,
    };

    class AmxxPlayerSubclass
        : public AmxxFeature<PlayerSubclassForward, PlayerSubclassForward::MAX_FORWARDS> {
      public:
        AmxxPlayerSubclass() : AmxxFeature("player_subclass") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto ChangeSubclassPre(Player& player, int playerClassIndex, int attackerIndex) -> ForwardReturn;

        auto ChangeSubclassPost(Player& player, int playerClassIndex, int attackerIndex) -> void;
    };

    inline AmxxPlayerSubclass amxxPlayerSubclass;
}
