#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxFlashlight : public AmxxFeature<> {
      public:
        AmxxFlashlight() : AmxxFeature("flashlight") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxFlashlight FlashlightApi;
}
