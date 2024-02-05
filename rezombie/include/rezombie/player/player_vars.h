#pragma once

#include "rezombie/player/long_jump_vars.h"
#include "rezombie/entity/models/models_pack.h"
#include "rezombie/preview/join_preview.h"
#include "rezombie/weapons/weapons.h"
#include <cssdk/engine/eiface.h>
#include <cssdk/public/regamedll/cs_player.h>
#include <cssdk/public/utils.h>

namespace rz
{
    // no need I think
    struct PlayerVars {
        int classId = 0;
        int subclassId = 0;
        std::unordered_map<int, int> subclasses;
        int propsId = 0;
        int modelId = 0;
        int soundsId = 0;
        int meleeId = 0;
        float freezeEndTime = 0.f;
    };
}
