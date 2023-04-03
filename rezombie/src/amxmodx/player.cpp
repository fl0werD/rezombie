#include "rezombie/amxmodx/player.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>
#include <cssdk/dll/player.h>

namespace rz {
    using namespace amxx;
    using namespace player;

    auto AmxxPlayer::PlayerGiveDefaultItems(Player& player) const -> ForwardReturn {
        return static_cast<ForwardReturn>(
            ExecuteForward(getForward(PlayerForward::GiveDefaultItems), player.id(), player.getClass())
        );
    }

    auto AmxxPlayer::ChooseDefaultSubclass(Player& player) const -> void {
        ExecuteForward(getForward(PlayerForward::ChooseDefaultSubclass), player.id());
    }

    auto AmxxPlayer::LongJumpState(Player& player) const -> void {
        ExecuteForward(getForward(PlayerForward::LongJumpState), player.id(), player.getLongJumpState());
    }

    auto AmxxPlayer::LongJumpActivated(Player& player) const -> void {
        ExecuteForward(getForward(PlayerForward::LongJumpActivated), player.id());
    }

    auto AmxxPlayer::RegisterForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            PlayerForward::GiveDefaultItems,
            RegisterForward("@rz_player_give_default_items", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            PlayerForward::ChooseDefaultSubclass,
            RegisterForward("@rz_player_choose_default_subclass", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            PlayerForward::LongJumpState,
            RegisterForward("@rz_player_long_jump_state", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            PlayerForward::LongJumpActivated,
            RegisterForward("@rz_player_long_jump_activated", e::Ignore, p::Cell, p::Done)
        );
    }

    auto AMX_NATIVE_CALL rz_get_player_var(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
            arg_4,
        };

        const int playerIndex = params[arg_player];
        const auto& player = players[playerIndex];
        using var = RzPlayerVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Team: {
                return player.getTeamId();
            }
            case var::Class: {
                return player.getClass();
            }
            case var::Subclass: {
                return player.getSubclass();
            }
            case var::ExtraJumps: {
                return player.getMaxExtraJumps();
            }
            default: {
                return false;
            }
        }
    }

    auto AMX_NATIVE_CALL rz_set_player_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_value,
        };

        const int playerIndex = params[arg_player];
        auto& player = players[playerIndex];
        using var = RzPlayerVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Team: {
                break;
            }
            case var::Class: {
                break;
            }
            case var::ExtraJumps: {
                player.setMaxExtraJumps(*Address(amx, params[arg_value]));
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_change_player_class(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_class,
            arg_attacker,
        };

        const int playerIndex = params[arg_player];
        const int playerClassIndex = params[arg_class];
        const int attackerIndex = params[arg_attacker];
        auto& player = players[playerIndex];
        if (attackerIndex != 0) {
            auto& attacker = players[attackerIndex];
            return player.ChangeClass(playerClassIndex, &attacker);
        }
        return player.ChangeClass(playerClassIndex);
    }

    auto AMX_NATIVE_CALL rz_set_preview_player_model(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_model_path,
        };

        const int playerIndex = params[arg_player];
        const auto handle = GetAmxString(amx, params[arg_model_path]);
        auto& player = players[playerIndex];
        // TODO: add change model
        return true;
    }

    auto AMX_NATIVE_CALL rz_subclass_selected(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerIndex = params[arg_player];
        auto& player = players[playerIndex];
        player.SelectAppearance();
        return true;
    }

    auto AMX_NATIVE_CALL rz_freeze_player(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_freeze_time,
            arg_in_air,
        };

        const int playerIndex = params[arg_player];
        const float freezeTime = CellToFloat(params[arg_freeze_time]);
        // const bool inAir = params[arg_in_air];
        auto& player = players[playerIndex];
        player.Freeze(freezeTime);
        return true;
    }

    auto AMX_NATIVE_CALL rz_is_player_freeze(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerIndex = params[arg_player];
        const auto& player = players[playerIndex];
        return player.isFrozen();
    }

    auto AMX_NATIVE_CALL rz_give_long_jump(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_force,
            arg_height,
            arg_cooldown,
        };

        const int playerIndex = params[arg_player];
        const int force = params[arg_force];
        const int height = params[arg_height];
        const float cooldown = CellToFloat(params[arg_cooldown]);
        auto& player = players[playerIndex];
        player.GiveLongJump(force, height, cooldown);
        return true;
    }

    auto AMX_NATIVE_CALL rz_remove_long_jump(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerIndex = params[arg_player];
        auto& player = players[playerIndex];
        player.RemoveLongJump();
        return true;
    }

    auto AmxxPlayer::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_get_player_var",           rz_get_player_var},
            {"rz_set_player_var",           rz_set_player_var},
            {"rz_change_player_class",      rz_change_player_class},
            {"rz_set_preview_player_model", rz_set_preview_player_model},
            {"rz_subclass_selected",        rz_subclass_selected},
            {"rz_freeze_player",            rz_freeze_player},
            {"rz_is_player_freeze",         rz_is_player_freeze},
            {"rz_give_long_jump",           rz_give_long_jump},
            {"rz_remove_long_jump",         rz_remove_long_jump},

            {nullptr,                       nullptr},
        };
        AddNatives(natives);
    }
}
