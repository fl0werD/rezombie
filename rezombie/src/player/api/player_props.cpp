#include "rezombie/player/modules/player_props.h"
#include "rezombie/player/api/player_props.h"
#include "rezombie/colors/colors.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class PropsVars : int {
        Handle,
        Health,
        BaseHealth,
        Armor,
        Helmet,
        Speed,
        Gravity,
        Footsteps,
        WeaponsInteraction,
        VelModBody,
        VelModHead,
        Knockback,
        BloodColor,
        RenderMode,
        RenderAmount,
        RenderColor,
        RenderFx,
    };

    const std::unordered_map<std::string, PropsVars> PropsVarsMap = {
        {"handle",              PropsVars::Handle},
        {"health",              PropsVars::Health},
        {"base_health",         PropsVars::BaseHealth},
        {"armor",               PropsVars::Armor},
        {"helmet",              PropsVars::Helmet},
        {"speed",               PropsVars::Speed},
        {"gravity",             PropsVars::Gravity},
        {"footsteps",           PropsVars::Footsteps},
        {"weapons_interaction", PropsVars::WeaponsInteraction},
        {"VelModBody",          PropsVars::VelModBody},
        {"VelModHead",          PropsVars::VelModHead},
        {"knockback",           PropsVars::Knockback},
        {"blood_color",         PropsVars::BloodColor},
        {"render_mode",         PropsVars::RenderMode},
        {"render_amount",       PropsVars::RenderAmount},
        {"render_color",        PropsVars::RenderColor},
        {"render_fx",           PropsVars::RenderFx},
    };

    auto create_props(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto propsId = Props.add(handle);
        return propsId;
    }

    auto HandlePropsVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_props,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = PropsVars;

        const auto propsRef = Props[params[arg_props]];
        if (!propsRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(PropsVarsMap, key);
        CHECK_VAR_EXISTS("Invalid props '%s' var", key)
        auto& props = propsRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], props.getHandle().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Health: {
                if (isGetter) {
                    return props.getHealth();
                } else {
                    props.setHealth(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::BaseHealth: {
                if (isGetter) {
                    return props.getBaseHealth();
                } else {
                    props.setBaseHealth(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Armor: {
                if (isGetter) {
                    return props.getArmor();
                } else {
                    props.setArmor(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Helmet: {
                if (isGetter) {
                    return props.getHelmet();
                } else {
                    props.setHelmet(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Speed: {
                if (isGetter) {
                    return props.getSpeed();
                } else {
                    props.setSpeed(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Gravity: {
                if (isGetter) {
                    return FloatToCell(props.getGravity());
                } else {
                    props.setGravity(CellToFloat(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::Footsteps: {
                if (isGetter) {
                    return props.getFootSteps();
                } else {
                    props.setFootSteps(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::WeaponsInteraction: {
                if (isGetter) {
                    return props.getWeaponsInteraction();
                } else {
                    props.setWeaponsInteraction(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::VelModBody: {
                if (isGetter) {
                    // return playerPropses.at(props).velModBody;
                } else {
                    // playerPropses.at(props).velModBody = *value;
                }
                break;
            }
            case vars::VelModHead: {
                if (isGetter) {
                    // return playerPropses.at(props).velModHead;
                } else {
                    // playerPropses.at(props).velModHead = *value;
                }
                break;
            }
            case vars::Knockback: {
                if (isGetter) {
                    // return playerPropses.at(props).knockback;
                } else {
                    // playerPropses.at(props).knockback = *value;
                }
                break;
            }
            case vars::BloodColor: {
                if (isGetter) {
                    return props.getBloodColor();
                } else {
                    props.setBloodColor(static_cast<byte>(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::RenderMode: {
                if (isGetter) {
                    return toInt(props.getRenderMode());
                } else {
                    props.setRenderMode(static_cast<Rendering>(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::RenderAmount: {
                if (isGetter) {
                    return props.getRenderAmount();
                } else {
                    props.setRenderAmount(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::RenderColor: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], props.getRenderColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    const auto colorRef = GetAmxString(amx, params[arg_3]);
                    auto color = Colors.parse(colorRef);
                    if (!color) {
                        return false;
                    }
                    props.setRenderColor(colorRef);
                }
                break;
            }
            case vars::RenderFx: {
                if (isGetter) {
                    return toInt(props.getRenderFx());
                } else {
                    props.setRenderFx(static_cast<RenderingFx>(*Address(amx, params[arg_3])));
                }
                break;
            }
        }
        return true;
    }

    auto get_props_var(Amx* amx, cell* params) -> cell {
        return HandlePropsVar(amx, params, true);
    }

    auto set_props_var(Amx* amx, cell* params) -> cell {
        return HandlePropsVar(amx, params, false);
    }

    auto AmxxPlayerProps::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_props",  create_props},
            {"get_props_var", get_props_var},
            {"set_props_var", set_props_var},

            {nullptr,         nullptr},
        };
        AddNatives(natives);
    }
}
