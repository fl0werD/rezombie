#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player/player_props.h"

namespace rz
{
    class Props : public Module<PlayerProps> {
      public:
        Props() : Module("player_props") {}

        auto add(std::string handle) -> int {
            return Module::add(new PlayerProps(std::move(handle)));
        }

        auto add(PlayerProps *props) -> int {
            return Module::add(props);
        }
    };

    inline Props Props;
}
