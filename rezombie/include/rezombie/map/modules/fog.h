#pragma once

#include <utility>

#include "rezombie/core/module.h"
#include "rezombie/entity/map/fog.h"

namespace rz
{
    class FogModule : public Module<Fog> {

      public:
        FogModule() : Module<Fog>("fogs") {}

        auto add(std::string color, int densityPercentage) -> int {
            return Module::add(new Fog(std::move(color), densityPercentage));
        }
    };

    inline FogModule Fogs;
}
