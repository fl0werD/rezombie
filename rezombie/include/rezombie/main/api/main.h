#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxMain : public AmxxFeature<> {
      public:
        AmxxMain() : AmxxFeature("main") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxMain MainApi;
}
