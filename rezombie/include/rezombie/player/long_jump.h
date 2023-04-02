#pragma once

#include <messages/engine_message.h>
#include "rezombie/player/player_model.h"
#include "rezombie/player/player_preview.h"
#include "rezombie/weapons/weapons.h"
#include <cssdk/engine/eiface.h>
#include <cssdk/public/regamedll/cs_player.h>
#include <cssdk/public/utils.h>

namespace rz::player
{
    enum class LongJumpState {
        None,
        Cooldown,
        Ready,
    };

    constexpr auto ITEM_LONG_JUMP = "item_longjump";
}
