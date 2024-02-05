#include "rezombie/modes/api/modes.h"
#include "rezombie/modes/modules/modes.h"
#include "rezombie/main/util.h"
#include "rezombie/core/api/amxx_helper.h"
#include <amxx/api.h>

#include "rezombie/gamerules/game_rules.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxModes::ModeStart(int modeId, int target) const -> void {
        executeForward(ModesForward::ModeStart, modeId, target);
    }

    auto AmxxModes::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            ModesForward::ModeStart,
            RegisterForward("@mode_start", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class ModeVars : int {
        Handle,
        Name,
        Description,
        NoticeMessage,
        Color,
        DropChance,
        MinimumPlayers,
        RoundTime,
        IsSupportTarget,
        Respawn,
    };

    const std::unordered_map<std::string, ModeVars> ModeVarsMap = {
        {"handle", ModeVars::Handle},
        {"name", ModeVars::Name},
        {"description", ModeVars::Description},
        {"notice_message", ModeVars::NoticeMessage},
        {"color", ModeVars::Color},
        {"drop_chance", ModeVars::DropChance},
        {"minimum_players", ModeVars::MinimumPlayers},
        {"round_time", ModeVars::RoundTime},
        {"support_target", ModeVars::IsSupportTarget},
        {"respawn", ModeVars::Respawn},
    };

    auto create_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_launch_forward,
            arg_is_support_target,
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
        const bool isSupportTarget = params[arg_is_support_target];
        const auto modeId = Modes.add(handle, launchForward, isSupportTarget);
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
        CHECK_VAR_EXISTS("Invalid mode '%s' var", key)
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
            case vars::Description: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getDescription().c_str(), *Address(amx, params[arg_4]));
                } else {
                    mode.setDescription(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::NoticeMessage: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getNoticeMessage().c_str(), *Address(amx, params[arg_4]));
                } else {
                    mode.setNoticeMessage(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Color: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], mode.getColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    mode.setColor(GetAmxString(amx, params[arg_3]));
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
            case vars::IsSupportTarget: {
                if (isGetter) {
                    return mode.isSupportTarget();
                } else {
                    mode.setSupportTarget(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Respawn: {
                if (isGetter) {
                    return toInt(mode.getRespawn());
                } else {
                    mode.setRespawn(static_cast<RespawnType>(*Address(amx, params[arg_3])));
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

    auto mode_add_ambience_sound(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_mode,
            arg_path,
        };

        const int modeId = params[arg_mode];
        const auto& modeRef = Modes[modeId];
        if (!modeRef) {
            // Invalid index
            return false;
        }
        auto& mode = modeRef->get();
        const auto path = GetAmxString(amx, params[arg_path]);
        mode.addAmbienceSound(path);
        return true;
    }

    auto mode_random_ambience_sound(Amx* amx, cell* params) -> cell {
        constexpr auto PLATFORM_MAX_PATH = 256;

        enum {
            arg_count,
            arg_mode,
            arg_return,
        };

        const int modeId = params[arg_mode];
        const auto& modeRef = Modes[modeId];
        if (!modeRef) {
            // Invalid index
            return false;
        }
        const auto& mode = modeRef->get();
        const auto& path = mode.getRandomAmbienceSound();
        SetAmxStringUtf8Char(amx, params[arg_return], path.c_str(), path.length(), PLATFORM_MAX_PATH);
        return true;
    }

    auto modes_count(Amx*, cell*) -> cell {
        return Modes.count();
    }

    auto modes_begin(Amx*, cell*) -> cell {
        return Modes.begin();
    }

    auto modes_end(Amx*, cell*) -> cell {
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

    auto launch_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_mode,
            arg_target,
        };

        const int modeId = params[arg_mode];
        const int target = params[arg_target];
        const auto modeRef = Modes[modeId];
        if (modeRef) {
            //setMode(modeId);
            const auto& mode = modeRef->get();
            /*if (mode.getRoundTime()) {
                setTimer(mode.getRoundTime());
            } else {
                setTimer(round_time);
            }*/
            const auto target2 = mode.executeLaunch(target);
            ModesApi.ModeStart(modeId, target2);
            GameRules.setRoundState(RoundState::Playing);
        }
        return true;
    }

    auto AmxxModes::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_mode", create_mode},
            {"get_mode_var", get_mode_var},
            {"set_mode_var", set_mode_var},
            {"mode_add_ambience_sound", mode_add_ambience_sound},
            {"mode_random_ambience_sound", mode_random_ambience_sound},
            {"modes_count", modes_count},
            {"modes_begin", modes_begin},
            {"modes_end", modes_end},
            {"find_mode", find_mode},
            {"launch_mode", launch_mode},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
