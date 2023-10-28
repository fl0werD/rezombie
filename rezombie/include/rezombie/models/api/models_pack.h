#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include <array>

namespace rz
{
    class AmxxModelsPack : public AmxxFeature<> {
      public:
        AmxxModelsPack() : AmxxFeature("models_pack") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxModelsPack ModelsPackApi;
}
