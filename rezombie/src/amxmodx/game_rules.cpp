#include "rezombie/amxmodx/game_rules.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/modules/game_mode.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxGameRules::RoundStart(bool isReset) -> void {
        ExecuteForward(getForward(GameRulesForward::RoundStart), isReset);
    }

    auto AmxxGameRules::RoundEnd(EndRoundEvent event, int gameModeIndex, int delay) -> void {
        ExecuteForward(getForward(GameRulesForward::RoundEnd), event, gameModeIndex, delay);
    }

    auto AmxxGameRules::RoundTimer(int time) -> void {
        ExecuteForward(getForward(GameRulesForward::RoundTimer), time);
    }

    auto AmxxGameRules::GameStateChanged(GameState oldGameState, GameState newGameState) -> void {
        ExecuteForward(getForward(GameRulesForward::GAME_STATE_CHANGED), oldGameState, newGameState);
    }

    auto AmxxGameRules::RoundStateChanged(RoundState oldRoundState, RoundState newRoundState) -> void {
        ExecuteForward(getForward(GameRulesForward::ROUND_STATE_CHANGED), oldRoundState, newRoundState);
    }

    auto AmxxGameRules::GameModeStart(int gameModeIndex) -> void {
        ExecuteForward(getForward(GameRulesForward::GAME_MODE_START), gameModeIndex);
    }

    auto AmxxGameRules::RegisterForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            GameRulesForward::RoundStart,
            RegisterForward("@rz_round_start", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::RoundEnd,
            RegisterForward("@rz_round_end", e::Ignore, p::Cell, p::Cell, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::RoundTimer,
            RegisterForward("@rz_round_hud_timer", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::GAME_STATE_CHANGED,
            RegisterForward("rz_game_state_changed", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::ROUND_STATE_CHANGED,
            RegisterForward("rz_round_state_changed", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            GameRulesForward::GAME_MODE_START,
            RegisterForward("rz_game_mode_start", e::Ignore, p::Cell, p::Done)
        );
    }

    auto AMX_NATIVE_CALL rz_get_game_state(Amx*, cell*) -> cell {
        const auto gameState = toInt(gameRules->getGameState());
        return gameState;
    }

    auto AMX_NATIVE_CALL rz_get_round_state(Amx*, cell*) -> cell {
        const auto roundState = toInt(gameRules->getRoundState());
        return roundState;
    }

    auto AMX_NATIVE_CALL rz_get_round_remaining_time(Amx*, cell*) -> cell {
        return gameRules->getRoundRemainingTime();
    }

    auto AMX_NATIVE_CALL rz_get_team_wins(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_team,
        };

        const auto team = static_cast<Team>(params[arg_team]);
        return gameRules->getTeamWins(team);
    }

    auto AMX_NATIVE_CALL rz_get_game_mode(Amx*, cell*) -> cell {
        enum {
            arg_count,
        };

        return gameRules->getGameMode();
    }

    auto AMX_NATIVE_CALL rz_get_default_class(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_team,
        };

        const auto team = static_cast<Team>(params[arg_team]);
        // CHECK_PLAYABLE_TEAM(team, false)
        if (gameRules->getDefaultPlayerClassOverride(team)) {
            return gameRules->getDefaultPlayerClassOverride(team);
        }
        return gameRules->getDefaultPlayerClass(team);
    }

    auto AMX_NATIVE_CALL rz_set_default_class(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_team,
            arg_class,
        };

        const auto team = static_cast<Team>(params[arg_team]);
        // CHECK_PLAYABLE_TEAM(team, false)
        const int playerClassIndex = params[arg_class];
        // new index = rz_module_get_valid_index(g_iModule, class);
        // CHECK_MODULE_VALID_INDEX(index, false)
        gameRules->setDefaultPlayerClassOverride(team, playerClassIndex);
        return true;
    }

    auto AMX_NATIVE_CALL rz_override_default_class(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_team,
            arg_class,
        };

        const auto team = static_cast<Team>(params[arg_team]);
        // CHECK_PLAYABLE_TEAM(team, false)
        const int playerClassIndex = params[arg_class];
        if (!playerClassIndex) {
            gameRules->setDefaultPlayerClassOverride(team, 0);
            return true;
        }
        // new index = rz_module_get_valid_index(g_iModule, class);
        // CHECK_MODULE_VALID_INDEX(index, false)
        gameRules->setDefaultPlayerClassOverride(team, playerClassIndex);
        return true;
    }

    auto AMX_NATIVE_CALL rz_create_game_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_launch_forward,
        };

        using p = ForwardParam;

        // Check arg count
        auto handle = GetAmxString(amx, params[arg_handle]);
        const auto launchForward = RegisterSpForwardByName(amx, GetAmxString(amx, params[arg_launch_forward]), p::Cell, p::Done);
        if (launchForward == -1) {
            return 0;
        }
        const auto gameModeIndex = gameModeModule.add(handle, launchForward);
        return gameModeIndex;
    }

    auto AMX_NATIVE_CALL rz_get_game_mode_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_game_mode,
            arg_var,
            arg_3,
            arg_4,
        };

        const int gameModeIndex = params[arg_game_mode];
        const auto gameModeRef = gameModeModule[gameModeIndex];
        if (!gameModeRef) {
            // Invalid index
            return false;
        }
        const auto& gameMode = gameModeRef->get();
        using var = GameModeVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], gameMode.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], gameMode.getName().c_str(), *Address(amx, params[arg_4]));
                break;
            }
            case var::DropChance: {
                return gameMode.getDropChance();
            }
            case var::MinimumPlayers: {
                return gameMode.getMinPlayers();
            }
            case var::RoundTime: {
                return gameMode.getRoundTime();
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_game_mode_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_game_mode,
            arg_var,
            arg_value,
        };

        const int gameModeIndex = params[arg_game_mode];
        const auto gameModeRef = gameModeModule[gameModeIndex];
        if (!gameModeRef) {
            // Invalid index
            return false;
        }
        auto& gameMode = gameModeRef->get();
        using var = GameModeVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Name: {
                gameMode.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::DropChance: {
                gameMode.setDropChance(*Address(amx, params[arg_value]));
                break;
            }
            case var::MinimumPlayers: {
                gameMode.setMinPlayers(*Address(amx, params[arg_value]));
            }
            case var::RoundTime: {
                gameMode.setRoundTime(*Address(amx, params[arg_value]));
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_game_mode_begin(Amx*, cell*) -> cell {
        return gameModeModule.begin();
    }

    auto AMX_NATIVE_CALL rz_game_mode_end(Amx*, cell*) -> cell {
        return gameModeModule.end();
    }

    auto AMX_NATIVE_CALL rz_find_game_mode(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return gameModeModule.findHandle(handle);
    }

    auto AmxxGameRules::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_get_game_state",           rz_get_game_state},
            {"rz_get_round_state",          rz_get_round_state},
            {"rz_get_round_remaining_time", rz_get_round_remaining_time},
            {"rz_get_team_wins",            rz_get_team_wins},
            {"rz_get_game_mode",            rz_get_game_mode},

            {"rz_get_default_class",        rz_get_default_class},
            {"rz_set_default_class",        rz_set_default_class},
            {"rz_override_default_class",   rz_override_default_class},

            {"rz_create_game_mode",         rz_create_game_mode},
            {"rz_get_game_mode_var",        rz_get_game_mode_var},
            {"rz_set_game_mode_var",        rz_set_game_mode_var},
            {"rz_game_mode_begin",          rz_game_mode_begin},
            {"rz_game_mode_end",            rz_game_mode_end},
            {"rz_find_game_mode",           rz_find_game_mode},

            {nullptr,                       nullptr},
        };
        AddNatives(natives);
    }
}
