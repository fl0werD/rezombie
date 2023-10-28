#include "rezombie/player/modules/player_class.h"
#include "rezombie/player/api/player_class.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxPlayerClass::ChangeClassPre(int player, int classId, int attacker) const -> ForwardReturn {
        return executeForward(PlayerClassForward::ChangeClassPre, player, classId, attacker);
    }

    auto AmxxPlayerClass::ChangeClassPost(int player, int classId, int attacker) const -> void {
        executeForward(PlayerClassForward::ChangeClassPost, player, classId, attacker);
    }

    auto AmxxPlayerClass::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        registerForward(PlayerClassForward::ChangeClassPre, "@change_class_pre", e::Continue, p::Cell, p::Cell, p::Cell);
        registerForward(PlayerClassForward::ChangeClassPost, "@change_class_post", e::Ignore, p::Cell, p::Cell, p::Cell);
    }

    auto create_class(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_team,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto team = static_cast<Team>(params[arg_team]);
        const auto classId = Classes.add(handle, team);
        return classId;
    }

    enum class PlayerClassVars : int {
        Handle,
        Name,
        Team,
        HudColor,
        Props,
        Models,
        Sounds,
        Melee,
        ForwardGiveDefaultItems,
    };

    const std::unordered_map<std::string, PlayerClassVars> PlayerClassVarsMap = {
        {"handle",                     PlayerClassVars::Handle},
        {"name",                       PlayerClassVars::Name},
        {"team",                       PlayerClassVars::Team},
        {"hud_color",                  PlayerClassVars::HudColor},
        {"props",                      PlayerClassVars::Props},
        {"models",                     PlayerClassVars::Models},
        {"sounds",                     PlayerClassVars::Sounds},
        {"melee",                      PlayerClassVars::Melee},
        {"forward_give_default_items", PlayerClassVars::ForwardGiveDefaultItems},
    };

    auto HandleClassVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_class,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = PlayerClassVars;
        using p = ForwardParam;

        const auto playerClassRef = Classes[params[arg_class]];
        if (!playerClassRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(PlayerClassVarsMap, key);
        CHECK_VAR_EXISTS("Invalid class '%s' var", key)
        auto& playerClass = playerClassRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], playerClass.getHandle().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Name: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], playerClass.getName().c_str(), *Address(amx, params[arg_4]));
                } else {
                    playerClass.setName(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Team: {
                if (isGetter) {
                    return toInt(playerClass.getTeam());
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::HudColor: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], playerClass.getHudColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    playerClass.setHudColor(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Props: {
                if (isGetter) {
                    return playerClass.getProps();
                } else {
                    playerClass.setProps(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Models: {
                if (isGetter) {
                    return playerClass.getModelsPack();
                } else {
                    playerClass.setModelsPack(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Sounds: {
                if (isGetter) {
                    return playerClass.getSounds();
                } else {
                    playerClass.setSounds(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Melee: {
                if (isGetter) {
                    return playerClass.getMelee();
                } else {
                    playerClass.setMelee(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::ForwardGiveDefaultItems: {
                if (isGetter) {
                    return false;
                }
                const auto giveDefaultItems = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_3]),
                    p::Cell, p::Done
                );
                if (giveDefaultItems == -1) {
                    return false;
                }
                playerClass.setGiveDefaultItemsForward(giveDefaultItems);
                break;
            }
        }
        return true;
    }

    auto get_class_var(Amx* amx, cell* params) -> cell {
        return HandleClassVar(amx, params, true);
    }

    auto set_class_var(Amx* amx, cell* params) -> cell {
        return HandleClassVar(amx, params, false);
    }

    auto class_begin(Amx*, cell*) -> cell {
        return Classes.begin();
    }

    auto class_end(Amx*, cell*) -> cell {
        return Classes.end();
    }

    auto find_class(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Classes[handle];
    }

    auto AmxxPlayerClass::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_class",  create_class},
            {"get_class_var", get_class_var},
            {"set_class_var", set_class_var},
            {"class_begin",   class_begin},
            {"class_end",     class_end},
            {"find_class",    find_class},

            {nullptr,         nullptr},
        };
        AddNatives(natives);
    }
}
