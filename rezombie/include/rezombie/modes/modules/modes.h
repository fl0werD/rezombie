#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/gamerules/mode.h"

namespace rz
{
    class ModesModule : public Module<Mode> {
      public:
        ModesModule() : Module("modes") {}

        auto add(std::string handle, int launchForward, bool isSupportTarget) -> int {
            return Module::add(new Mode(std::move(handle), launchForward, isSupportTarget));
        }
    };

    inline ModesModule Modes;
}
