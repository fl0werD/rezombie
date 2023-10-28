#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/gamerules/mode.h"

namespace rz
{
    class ModesModule : public Module<Mode> {
      public:
        ModesModule() : Module<Mode>("modes") {}

        auto add(std::string handle, int launchForward) -> int {
            return Module::add(new Mode(std::move(handle), launchForward));
        }
    };

    inline ModesModule Modes;
}
