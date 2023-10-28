#include "rezombie/gamerules/api/modes.h"
#include "rezombie/gamerules/modules/modes.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxModes::ModeStart(int modeId) -> void {
        executeForward(ModesForward::ModeStart, modeId);
    }

    auto AmxxModes::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            ModesForward::ModeStart,
            RegisterForward("@mode_start", e::Ignore, p::Cell, p::Done)
        );
    }

    enum class ModeVars : int {
        Handle,
        Name,
        HudColor,
        DropChance,
        MinimumPlayers,
        RoundTime,
    };

    const std::unordered_map<std::string, ModeVars> ModeVarsMap = {
        {"handle",          ModeVars::Handle},
        {"name",            ModeVars::Name},
        {"hud_color",       ModeVars::HudColor},
        {"drop_chance",     ModeVars::DropChance},
        {"minimum_players", ModeVars::MinimumPlayers},
        {"round_time",      ModeVars::RoundTime},
    };

    auto create_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_launch_forward,
        };

        using p = ForwardParam;

        // Check arg count
        auto handle = GetAmxString(amx, params[arg_handle], 0);
        const auto launchForward = RegisterSpForwardByName(
            amx, GetAmxString(amx, params[arg_launch_forward], 1),
            p::Cell, p::Done
        );
        if (launchForward == FORWARD_INVALID) {
            return 0;
        }
        const auto modeId = Modes.add(handle, launchForward);
        return modeId;
    }

    auto HandleModeVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_mode,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = ModeVars;

        const int modeId = params[arg_mode];
        const auto modeRef = Modes[modeId];
        if (!modeRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(ModeVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& mode = modeRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getHandle().c_str(), params[arg_4]);
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Name: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getName().c_str(), *Address(amx, params[arg_4]));
                } else {
                    mode.setName(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::HudColor: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getHudColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    mode.setHudColor(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::DropChance: {
                if (isGetter) {
                    return mode.getDropChance();
                } else {
                    mode.setDropChance(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::MinimumPlayers: {
                if (isGetter) {
                    return mode.getMinPlayers();
                } else {
                    mode.setMinPlayers(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::RoundTime: {
                if (isGetter) {
                    return mode.getRoundTime();
                } else {
                    mode.setRoundTime(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_mode_var(Amx* amx, cell* params) -> cell {
        return HandleModeVar(amx, params, true);
    }

    auto set_mode_var(Amx* amx, cell* params) -> cell {
        return HandleModeVar(amx, params, false);
    }

    auto mode_begin(Amx*, cell*) -> cell {
        return Modes.begin();
    }

    auto mode_end(Amx*, cell*) -> cell {
        return Modes.end();
    }

    auto find_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Modes[handle];
    }

    auto AmxxModes::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_mode",  create_mode},
            {"get_mode_var", get_mode_var},
            {"set_mode_var", set_mode_var},
            {"mode_begin",   mode_begin},
            {"mode_end",     mode_end},
            {"find_mode",    find_mode},

            {nullptr,        nullptr},
        };
        AddNatives(natives);
    }
}
