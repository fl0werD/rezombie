#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxCurrency : public AmxxFeature<> {
      public:
        AmxxCurrency() : AmxxFeature("currency") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxCurrency CurrencyApi;
}
