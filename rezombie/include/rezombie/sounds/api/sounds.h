#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxSounds : public AmxxFeature<> {
      public:
        AmxxSounds() : AmxxFeature("sounds") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxSounds SoundsApi;
}
