#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxMapExtras: public AmxxFeature<> {
      public:
        AmxxMapExtras() : AmxxFeature("map_extras") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxMapExtras MapExtrasApi;
}
