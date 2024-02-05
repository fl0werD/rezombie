#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/player/flashlight.h"

namespace rz
{
    class FlashlightModule : public Module<Flashlight> {
      public:
        FlashlightModule() : Module("flashlights") {}

        auto add(
            std::string color,
            int size,
            float drainTime,
            float chargeTime
        ) -> int {
            return Module::add(
                new Flashlight(
                    std::move(color),
                    size,
                    drainTime,
                    chargeTime
                )
            );
        }
    };

    inline FlashlightModule Flashlights;
}
