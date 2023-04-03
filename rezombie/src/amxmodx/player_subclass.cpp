#include "rezombie/modules/player_subclass.h"
#include "rezombie/amxmodx/common.h"
#include "rezombie/amxmodx/player_subclass.h"
#include "rezombie/amxmodx/utils.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>
#include <cssdk/dll/player.h>

namespace rz {
    using namespace amx;
    using namespace amxx;
    using namespace player;

    auto
    AmxxPlayerSubclass::ChangeSubclassPre(Player& player, int playerClassIndex, int attackerIndex) -> ForwardReturn {
        return static_cast<ForwardReturn>(ExecuteForward(
            getForward(PlayerSubclassForward::ChangeSubclassPre), player.id(), playerClassIndex, attackerIndex
        ));
    }

    auto AmxxPlayerSubclass::ChangeSubclassPost(Player& player, int playerClassIndex, int attackerIndex) -> void {
        ExecuteForward(
            getForward(PlayerSubclassForward::ChangeSubclassPost), player.id(), playerClassIndex, attackerIndex
        );
    }

    auto AmxxPlayerSubclass::RegisterForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            PlayerSubclassForward::ChangeSubclassPre,
            RegisterForward("@rz_change_class_pre", e::Continue, p::Cell, p::Cell, p::Cell, p::Done)
        );
        setForward(
            PlayerSubclassForward::ChangeSubclassPost,
            RegisterForward("@rz_change_class_post", e::Ignore, p::Cell, p::Cell, p::Cell, p::Done)
        );
    }

    auto AMX_NATIVE_CALL rz_create_subclass(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_class,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const int classIndex = params[arg_class];
        const auto subclassIndex = playerSubclassModule.add(handle, classIndex);
        return subclassIndex;
    }

    auto AMX_NATIVE_CALL rz_get_subclass_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_subclass,
            arg_var,
            arg_3,
            arg_4,
        };

        const int subclassIndex = params[arg_subclass];
        const auto subclassRef = playerSubclassModule[subclassIndex];
        if (!subclassRef) {
            // Invalid index
            return false;
        }
        const auto& subclass = subclassRef->get();
        using var = PlayerSubclassVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], subclass.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], subclass.getName().c_str(), params[arg_4]);
                break;
            }
            case var::Class: {
                return subclass.getClass();
            }
            case var::Description: {
                SetAmxString(amx, params[arg_3], subclass.getDescription().c_str(), params[arg_4]);
                break;
            }
            case var::HudColor: {
                SetAmxColor24(amx, params[arg_3], subclass.getHudColor());
                break;
            }
            case var::PlayerProps: {
                return subclass.getProps();
            }
            case var::PlayerModel: {
                return subclass.getModel();
            }
            case var::PlayerSound: {
                return subclass.getSound();
            }
            case var::Melee: {
                return subclass.getMelee();
            }
            case var::Nightvision: {
                return subclass.getNightVision();
            }
            default: {
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_subclass_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_subclass,
            arg_var,
            arg_value,
        };

        const int subclassIndex = params[arg_subclass];
        const auto subclassRef = playerSubclassModule[subclassIndex];
        if (!subclassRef) {
            // Invalid index
            return false;
        }
        auto& subclass = subclassRef->get();
        using var = PlayerSubclassVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Name: {
                subclass.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::Description: {
                subclass.setDescription(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::HudColor: {
                subclass.setHudColor(std::move(GetAmxColor24(amx, params[arg_value])));
                break;
            }
            case var::PlayerProps: {
                subclass.setProps(params[arg_value]);
                break;
            }
            case var::PlayerModel: {
                subclass.setModel(params[arg_value]);
                break;
            }
            case var::PlayerSound: {
                subclass.setSound(params[arg_value]);
                break;
            }
            case var::Melee: {
                subclass.setMelee(params[arg_value]);
                break;
            }
            case var::Nightvision: {
                subclass.setNightVision(params[arg_value]);
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_subclass_begin(Amx*, cell*) -> cell {
        return playerSubclassModule.begin();
    }

    auto AMX_NATIVE_CALL rz_subclass_end(Amx*, cell*) -> cell {
        return playerSubclassModule.end();
    }

    auto AMX_NATIVE_CALL rz_find_subclass(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return playerSubclassModule.findHandle(handle);
    }

    auto AmxxPlayerSubclass::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_create_subclass",  rz_create_subclass},
            {"rz_get_subclass_var", rz_get_subclass_var},
            {"rz_set_subclass_var", rz_set_subclass_var},
            {"rz_subclass_begin",   rz_subclass_begin},
            {"rz_subclass_end",     rz_subclass_end},
            {"rz_find_subclass",    rz_find_subclass},

            {nullptr,               nullptr},
        };
        AddNatives(natives);
    }
}
