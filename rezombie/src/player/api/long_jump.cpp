#include "rezombie/player/api/long_jump.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amxx;

    auto AmxxLongJump::Give(int player) const -> void {
        executeForward(LongJumpForward::Give, player);
    }

    auto AmxxLongJump::State(int player, int longJumpState) const -> void {
        executeForward(LongJumpForward::State, player, longJumpState);
    }

    auto AmxxLongJump::Activated(int player) const -> void {
        executeForward(LongJumpForward::Activated, player);
    }

    auto AmxxLongJump::CooldownTimer(int player, int timer) const -> void {
        executeForward(LongJumpForward::CooldownTimer, player, timer);
    }

    auto AmxxLongJump::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            LongJumpForward::Give,
            RegisterForward("@give_long_jump", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            LongJumpForward::State,
            RegisterForward("@long_jump_state", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            LongJumpForward::Activated,
            RegisterForward("@long_jump_activated", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            LongJumpForward::CooldownTimer,
            RegisterForward("@long_jump_cooldown_timer", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class RzLongJumpVars {
        State,
        NextStateTime,
        Force,
        Height,
        Cooldown,
        CooldownTimer,
    };

    const std::unordered_map<std::string, RzLongJumpVars> LongJumpVarsMap = {
        {"state",           RzLongJumpVars::State},
        {"next_state_time", RzLongJumpVars::NextStateTime},
        {"force",           RzLongJumpVars::Force},
        {"height",          RzLongJumpVars::Height},
        {"cooldown",        RzLongJumpVars::Cooldown},
        {"cooldown_timer",  RzLongJumpVars::CooldownTimer},
    };

    auto HandleLongJumpVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
        };

        using vars = RzLongJumpVars;

        const int playerId = params[arg_player];
        CHECK_PLAYER(playerId)
        auto& player = Players[playerId];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(LongJumpVarsMap, key);
        CHECK_VAR_EXISTS("Invalid long jump '%s' var", key)
        auto& longJump = player.LongJump();
        switch (*var) {
            case vars::State: {
                if (isGetter) {
                    return toInt(longJump.getState());
                } else {
                    longJump.setState(player, static_cast<LongJumpState>(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::NextStateTime: {
                if (isGetter) {
                    return FloatToCell(longJump.getNextStateTime());
                } else {
                    longJump.setNextStateTime(CellToFloat(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::Force: {
                if (isGetter) {
                    return longJump.getForce();
                } else {
                    longJump.setForce(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Height: {
                if (isGetter) {
                    return longJump.getHeight();
                } else {
                    longJump.setHeight(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Cooldown: {
                if (isGetter) {
                    return longJump.getCooldown();
                } else {
                    longJump.setCooldown(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::CooldownTimer: {
                if (isGetter) {
                    return longJump.getCooldownTimer();
                } else {
                    longJump.setCooldownTimer(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_long_jump_var(Amx* amx, cell* params) -> cell {
        return HandleLongJumpVar(amx, params, true);
    }

    auto set_long_jump_var(Amx* amx, cell* params) -> cell {
        return HandleLongJumpVar(amx, params, false);
    }

    auto give_long_jump(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_force,
            arg_height,
            arg_cooldown,
        };

        const int playerId = params[arg_player];
        const int force = params[arg_force];
        const int height = params[arg_height];
        const int cooldown = params[arg_cooldown];
        auto& player = Players[playerId];
        player.GiveLongJump(force, height, cooldown);
        return true;
    }

    auto remove_long_jump(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerId = params[arg_player];
        auto& player = Players[playerId];
        player.RemoveLongJump();
        return true;
    }

    auto AmxxLongJump::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_long_jump_var", get_long_jump_var},
            {"set_long_jump_var", set_long_jump_var},

            {"give_long_jump",    give_long_jump},
            {"remove_long_jump",  remove_long_jump},

            {nullptr,             nullptr},
        };
        AddNatives(natives);
    }
}
