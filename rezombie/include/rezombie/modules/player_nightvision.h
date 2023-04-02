#pragma once

#include "rezombie/core/module.h"
#include "rezombie/player/player_nightvision.h"

namespace rz
{
    using namespace player;

    class PlayerNightVisionModule : public Module<PlayerNightVision>
    {
      public:
        PlayerNightVisionModule() : Module<PlayerNightVision>("player_nightvision")
        {
        }

        auto add(std::string handle) -> int
        {
            return Module::add(PlayerNightVision(std::move(handle)));
        }
    };

    inline PlayerNightVisionModule playerNightVisionModule;
}
