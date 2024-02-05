#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player/nightvision.h"

namespace rz
{
    class NightVisionModule : public Module<NightVision> {
      public:
        NightVisionModule() : Module("nightvisions") {}

        auto add(std::string color, int fogId) -> int {
            return Module::add(new NightVision(std::move(color), fogId));
        }
    };

    inline NightVisionModule NightVisions;
}
