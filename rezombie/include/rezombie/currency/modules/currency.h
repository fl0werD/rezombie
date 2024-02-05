#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/currency/currency.h"

namespace rz
{
    class CurrencyModule : public Module<Currency> {
      public:
        CurrencyModule() : Module("currency") {}

        auto add(
            std::string handle,
            int getForward,
            int setForward,
            int formatForward
        ) -> int {
            return Module::add(
                new Currency(
                    std::move(handle),
                    getForward,
                    setForward,
                    formatForward
                )
            );
        }
    };

    inline CurrencyModule Currency;
}
