#include "rezombie/modules/player_class.h"
#include "rezombie/amxmodx/player_class.h"
#include "rezombie/amxmodx/utils.h"
#include "rezombie/player/player.h"
#include <amxx/api.h>

namespace rz {
    using namespace amx;
    using namespace amxx;
    using namespace player;

    auto
    AmxxPlayerClass::ChangeClassPre(Player& player, int playerClassIndex, int attackerIndex) const -> ForwardReturn {
        return static_cast<ForwardReturn>(
            ExecuteForward(getForward(PlayerClassForward::ChangeClassPre), player.id(), playerClassIndex, attackerIndex)
        );
    }

    auto AmxxPlayerClass::ChangeClassPost(Player& player, int playerClassIndex, int attackerIndex) const -> void {
        ExecuteForward(getForward(PlayerClassForward::ChangeClassPost), player.id(), playerClassIndex, attackerIndex);
    }

    auto AmxxPlayerClass::RegisterForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            PlayerClassForward::ChangeClassPre,
            RegisterForward("@rz_change_class_pre", e::Continue, p::Cell, p::Cell, p::Cell, p::Done)
        );
        setForward(
            PlayerClassForward::ChangeClassPost,
            RegisterForward("@rz_change_class_post", e::Ignore, p::Cell, p::Cell, p::Cell, p::Done)
        );
    }

    auto AMX_NATIVE_CALL rz_create_class(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_team,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto team = static_cast<Team>(params[arg_team]);
        const auto playerClassIndex = playerClassModule.add(handle, team);
        return playerClassIndex;
    }

    auto AMX_NATIVE_CALL rz_get_class_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_class,
            arg_var,
            arg_3,
            arg_4,
        };

        const int playerClassIndex = params[arg_class];
        const auto playerClassRef = playerClassModule[playerClassIndex];
        if (!playerClassRef) {
            // Invalid index
            return false;
        }
        const auto& playerClass = playerClassRef->get();
        using var = PlayerClassVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], playerClass.getHandle().c_str(), *Address(amx, params[arg_4]));
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], playerClass.getName().c_str(), *Address(amx, params[arg_4]));
                break;
            }
            case var::Team: {
                return toInt(playerClass.getTeam());
            }
            case var::HudColor: {
                SetAmxColor24(amx, params[arg_3], playerClass.getHudColor());
                break;
            }
            case var::PlayerProps: {
                return playerClass.getProps();
            }
            case var::PlayerModel: {
                return playerClass.getModel();
            }
            case var::PlayerSound: {
                return playerClass.getSound();
            }
            case var::Melee: {
                return playerClass.getMelee();
            }
            case var::Nightvision: {
                return playerClass.getNightVision();
            }
            default: {
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_class_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_class,
            arg_var,
            arg_value,
        };

        const int playerClassIndex = params[arg_class];
        const auto playerClassRef = playerClassModule[playerClassIndex];
        if (!playerClassRef) {
            // Invalid index
            return false;
        }
        auto& playerClass = playerClassRef->get();
        using var = PlayerClassVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Name: {
                playerClass.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::Team: {
                // Invalid set var
            }
            case var::HudColor: {
                playerClass.setHudColor(GetAmxColor24(amx, params[arg_value]));
                break;
            }
            case var::PlayerProps: {
                playerClass.setProps(params[arg_value]);
                break;
            }
            case var::PlayerModel: {
                playerClass.setModel(params[arg_value]);
                break;
            }
            case var::PlayerSound: {
                playerClass.setSound(params[arg_value]);
                break;
            }
            case var::Melee: {
                playerClass.setMelee(params[arg_value]);
                break;
            }
            case var::Nightvision: {
                playerClass.setNightVision(params[arg_value]);
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_class_begin(Amx*, cell*) -> cell {
        return playerClassModule.begin();
    }

    auto AMX_NATIVE_CALL rz_class_end(Amx*, cell*) -> cell {
        return playerClassModule.end();
    }

    auto AMX_NATIVE_CALL rz_find_class(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return playerClassModule.findHandle(handle);
    }

    auto AmxxPlayerClass::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_create_class",  rz_create_class},
            {"rz_get_class_var", rz_get_class_var},
            {"rz_set_class_var", rz_set_class_var},
            {"rz_class_begin",   rz_class_begin},
            {"rz_class_end",     rz_class_end},
            {"rz_find_class",    rz_find_class},

            {nullptr,            nullptr},
        };
        AddNatives(natives);
    }
}
