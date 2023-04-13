#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player_sound.h"

namespace rz::player
{
    class PlayerSoundModule : public Module<PlayerSound> {
      public:
        PlayerSoundModule() : Module<PlayerSound>("player_sound") {}

        auto add(std::string handle) -> int {
            return Module::add(new PlayerSound(std::move(handle)));
        }
    };

    inline PlayerSoundModule playerSoundModule;
}
