#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/currency/price.h"

namespace rz
{
    class PriceModule : public Module<Price> {
      public:
        PriceModule() : Module("prices") {}

        auto add(int currency, int amount) -> int {
            return Module::add(new Price(currency, amount));
        }
    };

    inline PriceModule Prices;
}
