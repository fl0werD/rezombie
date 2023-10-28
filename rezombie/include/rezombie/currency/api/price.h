#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxPrice : public AmxxFeature<> {
      public:
        AmxxPrice() : AmxxFeature("prices") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxPrice PriceApi;
}
