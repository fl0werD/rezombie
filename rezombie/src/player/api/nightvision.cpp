#include "rezombie/player/api/nightvision.h"
#include "rezombie/player/modules/nightvision.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class NightVisionVars : int {
        Color,
        Alpha,
        Fog,
    };

    const std::unordered_map<std::string, NightVisionVars> NightVisionVarsMap = {
        {"color", NightVisionVars::Color},
        {"alpha", NightVisionVars::Alpha},
        {"fog",   NightVisionVars::Fog},
    };

    auto create_night_vision(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_color,
            arg_radius,
            arg_fog,
        };

        // Check arg count
        const auto color = GetAmxString(amx, params[arg_color]);
        const int radius = params[arg_radius];
        const auto fogId = params[arg_fog];
        const auto nightVisionId = NightVisions.add(color, radius, fogId);
        return nightVisionId;
    }

    auto HandleNightVisionVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_night_vision,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = NightVisionVars;

        const int nightVisionId = params[arg_night_vision];
        const auto nightVisionRef = NightVisions[nightVisionId];
        if (!nightVisionRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(NightVisionVarsMap, key);
        CHECK_VAR_EXISTS("Invalid night vision '%s' var", key)
        auto& nightVision = nightVisionRef->get();
        switch (*var) {
            case vars::Color: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], nightVision.getColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // check valideate
                    nightVision.setColor(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Alpha: {
                if (isGetter) {
                    return nightVision.getAlphaPercentage();
                } else {
                    // check 0..100
                    nightVision.setAlphaPercentage(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Fog: {
                if (isGetter) {
                    return nightVision.getFog();
                } else {
                    // check validate
                    nightVision.setFog(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_night_vision_var(Amx* amx, cell* params) -> cell {
        return HandleNightVisionVar(amx, params, true);
    }

    auto set_night_vision_var(Amx* amx, cell* params) -> cell {
        return HandleNightVisionVar(amx, params, false);
    }

    auto AmxxNightVision::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_night_vision",  create_night_vision},
            {"get_night_vision_var", get_night_vision_var},
            {"set_night_vision_var", set_night_vision_var},

            {nullptr,                nullptr},
        };
        AddNatives(natives);
    }
}
