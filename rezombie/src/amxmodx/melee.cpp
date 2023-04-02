#include "rezombie/amxmodx/melee.h"
#include "rezombie/modules/melee.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;
    using namespace player;

    auto AMX_NATIVE_CALL rz_create_melee(Amx* amx, cell* params) -> cell
    {
        enum
        {
            arg_count,
            arg_handle,
        };

        // Check arg count
        auto handle = GetAmxString(amx, params[arg_handle]);
        const auto meleeIndex = meleeModule.add(handle);
        return meleeIndex;
    }

    auto AMX_NATIVE_CALL rz_get_melee_var(Amx* amx, cell* params) -> cell
    {
        enum
        {
            arg_count,
            arg_melee,
            arg_var,
            arg_3,
            arg_4,
        };

        const int meleeIndex = params[arg_melee];
        const auto meleeRef = meleeModule[meleeIndex];
        if (!meleeRef) {
            // Invalid index
            return false;
        }
        const auto& melee = meleeRef->get();
        using var = MeleeVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], melee.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::ViewModel: {
                SetAmxString(amx, params[arg_3], melee.getViewModel().c_str(), params[arg_4]);
                break;
            }
            case var::PlayerModel: {
                SetAmxString(amx, params[arg_3], melee.getPlayerModel().c_str(), params[arg_4]);
                break;
            }
            case var::WorldModel: {
                SetAmxString(amx, params[arg_3], melee.getWorldModel().c_str(), params[arg_4]);
                break;
            }
            case var::WeaponList: {
                // TODO
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], melee.getName().c_str(), params[arg_4]);
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_melee_var(Amx* amx, cell* params) -> cell
    {
        enum
        {
            arg_count,
            arg_melee,
            arg_var,
            arg_value,
        };

        const int meleeIndex = params[arg_melee];
        const auto meleeRef = meleeModule[meleeIndex];
        if (!meleeRef) {
            // Invalid index
            return false;
        }
        auto& melee = meleeRef->get();
        using var = MeleeVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::ViewModel: {
                melee.setViewModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::PlayerModel: {
                melee.setPlayerModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::WorldModel: {
                melee.setWorldModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::WeaponList: {
                // TODO
                break;
            }
            case var::Name: {
                melee.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_give_melee(Amx*, cell* params) -> cell
    {
        enum
        {
            arg_count,
            arg_player,
            arg_melee,
            // arg_give_type,
        };

        const int playerIndex = params[arg_player];
        const int meleeIndex = params[arg_melee];
        auto& player = players[playerIndex];
        player.GiveMelee(meleeIndex);
        return 1;
    }

    auto RegisterMeleeNatives() -> void
    {
        static AmxNativeInfo natives[] = {
          {"rz_create_melee",  rz_create_melee },
          {"rz_get_melee_var", rz_get_melee_var},
          {"rz_set_melee_var", rz_set_melee_var},
          {"rz_give_melee",    rz_give_melee   },

          {nullptr,            nullptr         },
        };
        AddNatives(natives);
    }
}
