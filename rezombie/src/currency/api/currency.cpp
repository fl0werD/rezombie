#include "rezombie/currency/api/currency.h"
#include "rezombie/currency/modules/currency.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;

    auto create_currency(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_get,
            arg_set,
            arg_format,
        };

        using p = ForwardParam;

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle], 0);
        const auto getForward = RegisterSpForwardByName(
            amx, GetAmxString(amx, params[arg_get], 1),
            p::Cell, p::Done
        );
        if (getForward == FORWARD_INVALID) {
            return false;
        }
        const auto setForward = RegisterSpForwardByName(
            amx, GetAmxString(amx, params[arg_set], 2),
            p::Cell, p::Cell, p::String, p::Done
        );
        if (setForward == FORWARD_INVALID) {
            return false;
        }
        const auto formatForward = RegisterSpForwardByName(
            amx, GetAmxString(amx, params[arg_format], 3),
            p::Cell, p::Cell, p::Array, p::Cell, p::Done
        );
        if (formatForward == FORWARD_INVALID) {
            return false;
        }
        const auto currencyId = Currency.add(handle, getForward, setForward, formatForward);
        return currencyId;
    }

    auto format_currency(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_currency,
            arg_amount,
            arg_is_short,
            arg_return,
        };

        constexpr auto MAX_CURRENCY_FORMAT_LENGTH = 32;

        // Check arg count
        const auto currencyId = params[arg_currency];
        const auto& currencyRef = Currency[currencyId];
        if (!currencyRef) {
            // throw
            return false;
        }
        const auto currency = currencyRef->get();
        const auto amount = params[arg_amount];
        const bool isShort = params[arg_is_short];
        const int textLength = MAX_CURRENCY_FORMAT_LENGTH;
        auto text = std::string(textLength, '\0');
        currency.executeFormat(amount, isShort, text, textLength);
        SetAmxString(amx, params[arg_return], text.c_str(), textLength);
        return true;
    }

    auto find_currency(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Currency[handle];
    }

    auto add_player_currency(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_currency,
            arg_amount,
            arg_reason,
            arg_reason_arguments,
        };

        // Check arg count
        const int playerId = params[arg_player];
        const auto& player = Players[playerId];
        const auto currencyId = params[arg_currency];
        const auto& currencyRef = Currency[currencyId];
        if (!currencyRef) {
            // throw
            return false;
        }
        const auto currency = currencyRef->get();
        auto length = 0;
        const auto reason = FormatAmxString(amx, params, arg_reason, &length);
        return currency.executeSet(player, currency.executeGet(player) + params[arg_amount], reason);
    }

    auto AmxxCurrency::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_currency",     create_currency},
            {"format_currency",     format_currency},
            {"find_currency",       find_currency},
            {"add_player_currency", add_player_currency},

            {nullptr,               nullptr},
        };
        AddNatives(natives);
    }
}
