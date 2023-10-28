#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxColors : public AmxxFeature<> {
      public:
        AmxxColors() : AmxxFeature("colors") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxColors ColorsApi;
}
