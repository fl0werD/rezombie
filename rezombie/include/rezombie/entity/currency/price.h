#pragma once

namespace rz
{
    class Price {
        int currencyId_;
        int amount_;

      public:
        Price(
            int currencyId,
            int amount
        ) : currencyId_(currencyId),
            amount_(amount) {}

        auto getCurrency() const -> int { return currencyId_; }

        auto setCurrency(int currencyId) -> void { currencyId_ = currencyId; }

        auto getAmount() const -> int { return amount_; }

        auto setAmount(int amount) -> void { amount_ = amount; }
    };
}
