#include "rezombie/sounds/sounds.h"
#include "rezombie/player/players.h"
#include <core/rehlds_api.h>

namespace rz
{
    auto Sounds::playAmbienceSound(const std::string& soundPath) -> void {
        stopAmbienceSound();
        lastAmbienceSound_ = soundPath;
        Players.forEachConnected([&soundPath](const auto& player) {
            rehlds_api::Funcs()->sv_emit_sound2(
                player,
                player,
                SoundChannel::Auto,
                soundPath.c_str(),
                VOL_NORM,
                ATTN_NORM,
                0,
                PITCH_NORM,
                0,
                nullptr
            );
        });
    }

    auto Sounds::stopAmbienceSound() -> void {
        if (lastAmbienceSound_.empty()) {
            return;
        }
        Players.forEachConnected([this](const auto& player) {
            rehlds_api::Funcs()->sv_emit_sound2(
                player,
                player,
                SoundChannel::Auto,
                lastAmbienceSound_.c_str(),
                VOL_NORM,
                ATTN_NORM,
                SND_STOP,
                PITCH_NORM,
                0,
                nullptr
            );
        });
        lastAmbienceSound_.clear();
    }
}
