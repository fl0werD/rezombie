#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include <array>

namespace rz
{
    class AmxxModels : public AmxxFeature<> {
      public:
        AmxxModels() : AmxxFeature("models") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxModels ModelsApi;
}
