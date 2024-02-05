#include "rezombie/sounds/api/sounds.h"
#include "rezombie/sounds/sounds.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;

    auto play_ambience_sound(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_sound_path,
        };

        const auto soundPath = GetAmxString(amx, params[arg_sound_path]);
        Sounds.playAmbienceSound(soundPath);
        return true;
    }

    auto stop_ambience_sound(Amx* amx, cell* params) -> cell {
        Sounds.stopAmbienceSound();
        return true;
    }

    auto AmxxSounds::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"play_ambience_sound", play_ambience_sound},
            {"stop_ambience_sound", stop_ambience_sound},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
