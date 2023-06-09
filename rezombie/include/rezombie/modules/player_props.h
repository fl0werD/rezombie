#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player_props.h"

namespace rz
{
    using namespace player;

    class PlayerPropsModule : public Module<PlayerProps> {
      public:
        PlayerPropsModule() : Module<PlayerProps>("player_props") {}

        auto add(std::string handle) -> int {
            return Module::add(new PlayerProps(std::move(handle)));
        }
    };

    inline PlayerPropsModule playerPropsModule;
}
