#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player/player_sounds.h"

namespace rz
{
    class PlayerSoundModule : public Module<PlayerSounds> {
      public:
        PlayerSoundModule() : Module<PlayerSounds>("player_sound") {}

        auto add(std::string handle) -> int {
            return Module::add(new PlayerSounds(std::move(handle)));
        }
    };

    inline PlayerSoundModule PlayerSounds;
}
