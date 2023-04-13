#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/game_mode.h"

namespace rz
{
    class GameModeModule : public Module<GameMode> {
      public:
        GameModeModule() : Module<GameMode>("gamemode") {}

        auto add(std::string handle, int launchForward) -> int {
            return Module::add(new GameMode(std::move(handle), launchForward));
        }
    };

    inline GameModeModule gameModeModule;
}
