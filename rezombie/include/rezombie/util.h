#pragma once

#include "rezombie/gamerules/consts.h"
#include <cssdk/engine/edict.h>
#include <string>
#include <common/util.h>

namespace rz {
    using namespace cssdk;
    using namespace common;

    enum class PlayersCountType : int {
        All,
        Dead,
        Alive,
    };

    constexpr auto toUChar(int value) -> unsigned char {
        return static_cast<unsigned char>(value);
    }

    auto isPlayableTeam(Team team) -> bool;

    auto getPlayersCount(Team team, PlayersCountType countType) -> int;

    auto FileExists(const char* filePath) -> bool;

    auto UTIL_EmitSound(
        Edict* entity, SoundChannel channel, const std::string& sample, float volume = VOL_NORM,
        float attenuation = ATTN_NORM, int flags = 0, int pitch = PITCH_NORM
    ) -> void;

    auto UTIL_SendMotd(Edict* receiver, const std::string& text) -> void;

    auto UTIL_SendShowMenu(Edict* receiver, int keys, int time, const std::string& text) -> void;
}
