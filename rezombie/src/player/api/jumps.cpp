#include "rezombie/player/api/jumps.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amxx;

    auto AmxxJumps::Jump(int player, int count) const -> void {
        executeForward(JumpsForward::Jump, player, count);
    }

    auto AmxxJumps::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            JumpsForward::Jump,
            RegisterForward("@player_jump", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class RzJumpsVars {
        Count,
        Maximum,
    };

    const std::unordered_map<std::string, RzJumpsVars> JumpsVarsMap = {
        {"count",   RzJumpsVars::Count},
        {"maximum", RzJumpsVars::Maximum},
    };

    auto HandleJumpsVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
        };

        using vars = RzJumpsVars;

        const int playerId = params[arg_player];
        CHECK_PLAYER(playerId)
        auto& player = Players[playerId];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(JumpsVarsMap, key);
        CHECK_VAR_EXISTS("Invalid jumps '%s' var", key)
        auto& jumps = player.Jumps();
        switch (*var) {
            case vars::Count: {
                if (isGetter) {
                    return jumps.getCount();
                } else {
                    jumps.setCount(player, *Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Maximum: {
                if (isGetter) {
                    return jumps.getMaximum();
                } else {
                    jumps.setMaximum(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_jumps_var(Amx* amx, cell* params) -> cell {
        return HandleJumpsVar(amx, params, true);
    }

    auto set_jumps_var(Amx* amx, cell* params) -> cell {
        return HandleJumpsVar(amx, params, false);
    }

    auto AmxxJumps::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_jumps_var", get_jumps_var},
            {"set_jumps_var", set_jumps_var},

            {nullptr,               nullptr},
        };
        AddNatives(natives);
    }
}
