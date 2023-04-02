#pragma once

#include <messages/engine_message.h>
#include "rezombie/player/long_jump.h"
#include "rezombie/player/player_model.h"
#include "rezombie/player/player_preview.h"
#include "rezombie/weapons/weapons.h"
#include <cssdk/engine/eiface.h>
#include <cssdk/public/regamedll/cs_player.h>
#include <cssdk/public/utils.h>

namespace rz::player
{
    struct PlayerVars
    {
        int classIndex = 0;
        int subclassIndex = 0;
        int propsIndex = 0;
        int modelIndex = 0;
        int soundIndex = 0;
        int meleeIndex = 0;
        int nightVisionIndex = 0;
        int extraJumpsCount = 0;
        int maxExtraJumps = 0;
        LongJumpState longJumpState = LongJumpState::None;
        float longJumpNextStateTime = 0.f;
        int longJumpForce = 0;
        int longJumpHeight = 0;
        float longJumpCooldown = 0.f;
    };
}
