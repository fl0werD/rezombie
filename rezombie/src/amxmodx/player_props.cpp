#include "rezombie/modules/player_props.h"
#include "rezombie/amxmodx/player_props.h"
#include "rezombie/util.h"
#include <amxx/api.h>

namespace rz {
    using namespace amx;
    using namespace amxx;

    auto AMX_NATIVE_CALL rz_create_props(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto playerPropsIndex = playerPropsModule.add(handle);
        return playerPropsIndex;
    }

    auto AMX_NATIVE_CALL rz_get_props_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_props,
            arg_var,
            arg_3,
            arg_4,
        };

        const int playerPropsIndex = params[arg_props];
        const auto playerPropsRef = playerPropsModule[playerPropsIndex];
        if (!playerPropsRef) {
            // Invalid index
            return false;
        }
        const auto& playerProps = playerPropsRef->get();
        using var = PlayerPropsVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], playerProps.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::Health: {
                return playerProps.getHealth();
            }
            case var::BaseHealth: {
                return playerProps.getBaseHealth();
            }
            case var::Armor: {
                return playerProps.getArmor();
            }
            case var::Helmet: {
                return playerProps.getHelmet();
            }
            case var::Speed: {
                return playerProps.getSpeed();
            }
            case var::Gravity: {
                return FloatToCell(playerProps.getGravity());
            }
            case var::Footsteps: {
                return playerProps.getFootSteps();
            }
            case var::WeaponsInteraction: {
                return playerProps.getWeaponsInteraction();
            }
            case var::VelModBody: {
                // return playerPropses.at(props).velModBody;
            }
            case var::VelModHead: {
                // return playerPropses.at(props).velModHead;
            }
            case var::Knockback: {
                // return playerPropses.at(props).knockback;
            }
            case var::BloodColor: {
                return playerProps.getBloodColor();
            }
            case var::RenderMode: {
                return toInt(playerProps.getRenderMode());
            }
            case var::RenderAmount: {
                return playerProps.getRenderAmount();
            }
            case var::RenderColor: {
                const auto& color = playerProps.getRenderColor();
                const auto returnValue = Address(amx, params[arg_3]);
                returnValue[0] = FloatToCell(color.red);
                returnValue[1] = FloatToCell(color.green);
                returnValue[2] = FloatToCell(color.blue);
                return true;
            }
            case var::RenderFx: {
                return toInt(playerProps.getRenderFx());
            }
            default: {
                return 0;
            }
        }
        return 1;
    }

    auto AMX_NATIVE_CALL rz_set_props_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_props,
            arg_var,
            arg_value,
        };

        const int playerPropsIndex = params[arg_props];
        const auto playerPropsRef = playerPropsModule[playerPropsIndex];
        if (!playerPropsRef) {
            // Invalid index
            return false;
        }
        auto& playerProps = playerPropsRef->get();
        using var = PlayerPropsVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Health: {
                playerProps.setHealth(*Address(amx, params[arg_value]));
                break;
            }
            case var::BaseHealth: {
                playerProps.setBaseHealth(*Address(amx, params[arg_value]));
                break;
            }
            case var::Armor: {
                playerProps.setArmor(*Address(amx, params[arg_value]));
                break;
            }
            case var::Helmet: {
                playerProps.setHelmet(*Address(amx, params[arg_value]));
                break;
            }
            case var::Speed: {
                playerProps.setSpeed(*Address(amx, params[arg_value]));
                break;
            }
            case var::Gravity: {
                playerProps.setGravity(CellToFloat(*Address(amx, params[arg_value])));
                break;
            }
            case var::Footsteps: {
                playerProps.setFootSteps(*Address(amx, params[arg_value]));
                break;
            }
            case var::WeaponsInteraction: {
                playerProps.setWeaponsInteraction(*Address(amx, params[arg_value]));
                break;
            }
            case var::VelModBody: {
                // playerPropses.at(props).velModBody = *value;
                break;
            }
            case var::VelModHead: {
                // playerPropses.at(props).velModHead = *value;
                break;
            }
            case var::Knockback: {
                // playerPropses.at(props).knockback = *value;
                break;
            }
            case var::BloodColor: {
                playerProps.setBloodColor(static_cast<byte>(*Address(amx, params[arg_value])));
                break;
            }
            case var::RenderMode: {
                playerProps.setRenderMode(static_cast<Rendering>(*Address(amx, params[arg_value])));
                break;
            }
            case var::RenderAmount: {
                playerProps.setRenderAmount(*Address(amx, params[arg_value]));
                break;
            }
            case var::RenderColor: {
                const auto value = Address(amx, params[arg_value]);
                const auto color = Color24{toUChar(value[0]), toUChar(value[1]), toUChar(value[2])};
                playerProps.setRenderColor(color);
                break;
            }
            case var::RenderFx: {
                playerProps.setRenderFx(static_cast<RenderingFx>(*Address(amx, params[arg_value])));
                break;
            }
            default: {
                return false;
            }
        }
        return true;
    }

    auto AmxxPlayerProps::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_create_props",  rz_create_props},
            {"rz_get_props_var", rz_get_props_var},
            {"rz_set_props_var", rz_set_props_var},

            {nullptr,            nullptr},
        };
        AddNatives(natives);
    }
}
