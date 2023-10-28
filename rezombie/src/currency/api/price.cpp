#include "rezombie/currency/modules/price.h"
#include "rezombie/currency/api/price.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class PriceVars : int {
        Currency,
        Amount,
    };

    const std::unordered_map<std::string, PriceVars> PriceVarsMap = {
        {"currency", PriceVars::Currency},
        {"amount",   PriceVars::Amount},
    };

    auto create_price(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_currency,
            arg_amount,
        };

        // Check arg count
        const auto currencyId = params[arg_currency];
        const auto amount = params[arg_amount];
        return Prices.add(currencyId, amount);
    }

    auto HandlePriceVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_price,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = PriceVars;

        const int priceId = params[arg_price];
        const auto priceRef = Prices[priceId];
        if (!priceRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(PriceVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& price = priceRef->get();
        switch (*var) {
            case vars::Currency: {
                if (isGetter) {
                    return price.getCurrency();
                } else {
                    price.setCurrency(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Amount: {
                if (isGetter) {
                    return price.getAmount();
                } else {
                    price.setAmount(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_price_var(Amx* amx, cell* params) -> cell {
        return HandlePriceVar(amx, params, true);
    }

    auto set_price_var(Amx* amx, cell* params) -> cell {
        return HandlePriceVar(amx, params, false);
    }

    auto AmxxPrice::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_price",  create_price},
            {"get_price_var", get_price_var},
            {"set_price_var", set_price_var},

            {nullptr,         nullptr},
        };
        AddNatives(natives);
    }
}
