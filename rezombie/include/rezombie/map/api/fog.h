#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxFog : public AmxxFeature<> {
      public:
        AmxxFog() : AmxxFeature("fog") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxFog FogApi;
}
