#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player/player_subclass.h"

namespace rz
{
    class PlayerSubclassModule : public Module<PlayerSubclass> {
      public:
        PlayerSubclassModule() : Module("player_subclass") {}

        auto add(std::string handle, int classIndex) -> int {
            return Module::add(new PlayerSubclass(std::move(handle), classIndex));
        }
    };

    inline PlayerSubclassModule Subclasses;
}
