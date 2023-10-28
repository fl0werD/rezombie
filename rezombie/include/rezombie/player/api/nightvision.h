#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxNightVision : public AmxxFeature<> {
      public:
        AmxxNightVision() : AmxxFeature("nigthvision") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxNightVision NightVisionApi;
}
