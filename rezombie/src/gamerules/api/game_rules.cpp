#include "rezombie/gamerules/api/game_rules.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/modes/modules/modes.h"
#include "rezombie/core/api/amxx_helper.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxGameRules::RoundStart(bool isReset) -> void {
        executeForward(GameRulesForward::RoundStart, isReset);
    }

    auto AmxxGameRules::RoundEnd(EndRoundEvent event, int modeId, int delay) -> void {
        executeForward(GameRulesForward::RoundEnd, event, modeId, delay);
    }

    auto AmxxGameRules::RoundTimer(int time) -> void {
        executeForward(GameRulesForward::RoundTimer, time);
    }

    auto AmxxGameRules::GameStateChanged(GameState oldGameState, GameState newGameState) -> void {
        executeForward(GameRulesForward::GameStateChanged, oldGameState, newGameState);
    }

    auto AmxxGameRules::RoundStateChanged(RoundState oldRoundState, RoundState newRoundState) -> void {
        executeForward(GameRulesForward::RoundStateChanged, oldRoundState, newRoundState);
    }

    auto AmxxGameRules::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            GameRulesForward::RoundStart,
            RegisterForward("@round_start", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::RoundEnd,
            RegisterForward("@round_end", e::Ignore, p::Cell, p::Cell, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::RoundTimer,
            RegisterForward("@round_timer", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::GameStateChanged,
            RegisterForward("@game_state_changed", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::RoundStateChanged,
            RegisterForward("@round_state_changed", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class GameVars : int {
        GameState,
        RoundState,
        Timer,
        TeamWins,
        Mode,
        DefaultClass,
        OverrideDefaultClass,
    };

    const std::unordered_map<std::string, GameVars> GameVarsMap = {
        {"game_state",             GameVars::GameState},
        {"round_state",            GameVars::RoundState},
        {"timer",                  GameVars::Timer},
        {"team_wins",              GameVars::TeamWins},
        {"mode",                   GameVars::Mode},
        {"default_class",          GameVars::DefaultClass},
        {"override_default_class", GameVars::OverrideDefaultClass},
    };

    auto HandleGameVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_var,
            arg_2,
            arg_3,
        };

        using vars = GameVars;
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(GameVarsMap, key);
        CHECK_VAR_EXISTS("Invalid game '%s' var", key)
        switch (*var) {
            case vars::GameState: {
                if (isGetter) {
                    return toInt(GameRules.getGameState());
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::RoundState: {
                if (isGetter) {
                    return toInt(GameRules.getRoundState());
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Timer: {
                if (isGetter) {
                    return GameRules.getTimer();
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::TeamWins: {
                const auto team = static_cast<Team>(*Address(amx, params[arg_2]));
                if (isGetter) {
                    return GameRules.getTeamWins(team);
                } else {
                    GameRules.setTeamWins(team, *Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Mode: {
                if (isGetter) {
                    return GameRules.getMode();
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::DefaultClass: {
                const auto team = static_cast<Team>(*Address(amx, params[arg_2]));
                // CHECK_PLAYABLE_TEAM(team, false)
                if (isGetter) {
                    if (GameRules.getDefaultPlayerClassOverride(team)) {
                        return GameRules.getDefaultPlayerClassOverride(team);
                    }
                    return GameRules.getDefaultPlayerClass(team);
                } else {
                    const int classId = *Address(amx, params[arg_3]);
                    // new index = rz_module_get_valid_index(g_iModule, class);
                    // CHECK_MODULE_VALID_INDEX(index, false)
                    GameRules.setDefaultPlayerClassOverride(team, classId);
                }
                break;
            }
            case vars::OverrideDefaultClass: {
                if (isGetter) {
                    // Invalid get vars
                } else {
                    const auto team = static_cast<Team>(*Address(amx, params[arg_2]));
                    // CHECK_PLAYABLE_TEAM(team, false)
                    const int classId = *Address(amx, params[arg_3]);
                    if (!classId) {
                        GameRules.setDefaultPlayerClassOverride(team, 0);
                        return true;
                    }
                    // new index = rz_module_get_valid_index(g_iModule, class);
                    // CHECK_MODULE_VALID_INDEX(index, false)
                    GameRules.setDefaultPlayerClassOverride(team, classId);
                }
                break;
            }
        }
        return true;
    }

    auto get_game_var(Amx* amx, cell* params) -> cell {
        return HandleGameVar(amx, params, true);
    }

    auto set_game_var(Amx* amx, cell* params) -> cell {
        return HandleGameVar(amx, params, false);
    }

    auto get_players_count(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_flags,
        };

        const auto flags = static_cast<PlayersCountFlags>(params[arg_flags]);
        return getPlayersCount(flags);
    }

    auto AmxxGameRules::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_game_var",      get_game_var},
            {"set_game_var",      set_game_var},

            {"get_players_count", get_players_count},

            {nullptr,             nullptr},
        };
        AddNatives(natives);
    }
}
