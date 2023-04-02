#pragma once

#include "rezombie/core/module.h"
#include "rezombie/gamerules/game_mode.h"

namespace rz
{
    class GameModeModule : public Module<GameMode>
    {
      public:
        GameModeModule() : Module<GameMode>("gamemode")
        {
        }

        auto add(std::string handle) -> int
        {
            return Module::add(GameMode(std::move(handle)));
        }
    };

    inline GameModeModule gameModeModule;
}
