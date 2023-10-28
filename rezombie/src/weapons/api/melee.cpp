#include "rezombie/weapons/api/melee.h"
#include "rezombie/player/players.h"
#include "amxx/api.h"

namespace rz
{
    using namespace amxx;
/*
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

        const int meleeId = params[arg_melee];
        const auto meleeRef = meleeModule[meleeId];
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
            case var::ModelsPack: {
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

        const int meleeId = params[arg_melee];
        const auto meleeRef = meleeModule[meleeId];
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
            case var::ModelsPack: {
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
    }*/

    auto AmxxMelee::registerNatives() const -> void
    {
        static AmxNativeInfo natives[] = {
          //{"rz_get_melee_var", rz_get_melee_var},
          //{"rz_set_melee_var", rz_set_melee_var},

          {nullptr,            nullptr         },
        };
        AddNatives(natives);
    }
}
