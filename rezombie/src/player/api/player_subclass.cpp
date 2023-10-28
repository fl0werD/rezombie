#include "rezombie/player/modules/player_subclass.h"
#include "rezombie/player/api/player_subclass.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxPlayerSubclass::ChangeSubclassPre(int player, int subclassId) const -> ForwardReturn {
        return executeForward(PlayerSubclassForward::ChangeSubclassPre, player, subclassId);
    }

    auto AmxxPlayerSubclass::ChangeSubclassPost(int player, int subclassId) -> void {
        executeForward(PlayerSubclassForward::ChangeSubclassPost, player, subclassId);
    }

    auto AmxxPlayerSubclass::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            PlayerSubclassForward::ChangeSubclassPre,
            RegisterForward("@change_subclass_pre", e::Continue, p::Cell, p::Cell, p::Done)
        );
        setForward(
            PlayerSubclassForward::ChangeSubclassPost,
            RegisterForward("@change_subclass_post", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class PlayerSubclassVars : int {
        Handle,
        Class,
        Name,
        Description,
        HudColor,
        Props,
        Model,
        Sounds,
        Melee,
    };

    const std::unordered_map<std::string, PlayerSubclassVars> PlayerSubclassVarsMap = {
        {"handle",      PlayerSubclassVars::Handle},
        {"class",       PlayerSubclassVars::Class},
        {"name",        PlayerSubclassVars::Name},
        {"description", PlayerSubclassVars::Description},
        {"hud_color",   PlayerSubclassVars::HudColor},
        {"props",       PlayerSubclassVars::Props},
        {"model",       PlayerSubclassVars::Model},
        {"sounds",      PlayerSubclassVars::Sounds},
        {"melee",       PlayerSubclassVars::Melee},
    };

    auto create_subclass(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_class,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const int classId = params[arg_class];
        const auto subclassId = Subclasses.add(handle, classId);
        return subclassId;
    }

    auto HandleSubclassVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_subclass,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = PlayerSubclassVars;

        const auto subclassRef = Subclasses[params[arg_subclass]];
        if (!subclassRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(PlayerSubclassVarsMap, key);
        CHECK_VAR_EXISTS("Invalid subclass '%s' var", key)
        auto& subclass = subclassRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], subclass.getHandle().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Class: {
                if (isGetter) {
                    return subclass.getClass();
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Name: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], subclass.getName().c_str(), *Address(amx, params[arg_4]));
                } else {
                    subclass.setName(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Description: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], subclass.getDescription().c_str(), *Address(amx, params[arg_4]));
                } else {
                    subclass.setDescription(GetAmxString(amx, params[3]));
                }
                break;
            }
            case vars::HudColor: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], subclass.getHudColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    subclass.setHudColor(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Props: {
                if (isGetter) {
                    return subclass.getProps();
                } else {
                    subclass.setProps(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Model: {
                if (isGetter) {
                    return subclass.getModel();
                } else {
                    subclass.setModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Sounds: {
                if (isGetter) {
                    return subclass.getSounds();
                } else {
                    subclass.setSounds(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Melee: {
                if (isGetter) {
                    return subclass.getMelee();
                } else {
                    subclass.setMelee(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_subclass_var(Amx* amx, cell* params) -> cell {
        return HandleSubclassVar(amx, params, true);
    }

    auto set_subclass_var(Amx* amx, cell* params) -> cell {
        return HandleSubclassVar(amx, params, false);
    }

    auto subclass_begin(Amx*, cell*) -> cell {
        return Subclasses.begin();
    }

    auto subclass_end(Amx*, cell*) -> cell {
        return Subclasses.end();
    }

    auto find_subclass(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Subclasses[handle];
    }

    auto AmxxPlayerSubclass::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_subclass",  create_subclass},
            {"get_subclass_var", get_subclass_var},
            {"set_subclass_var", set_subclass_var},
            {"subclass_begin",   subclass_begin},
            {"subclass_end",     subclass_end},
            {"find_subclass",    find_subclass},

            {nullptr,            nullptr},
        };
        AddNatives(natives);
    }
}
