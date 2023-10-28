#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxEnvironment : public AmxxFeature<> {
      public:
        AmxxEnvironment() : AmxxFeature("environment") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxEnvironment EnvironmentApi;
}
