#include "rezombie/map/api/fog.h"
#include "rezombie/map/modules/fog.h"
#include "rezombie/colors/colors.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>
#include <unordered_map>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class FogVars : int {
        Color,
        DensityPercentage,
    };

    const std::unordered_map<std::string, FogVars> FogVarsMap = {
        {"color",   FogVars::Color},
        {"density", FogVars::DensityPercentage},
    };

    auto create_fog(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_color,
            arg_density_percentage,
        };

        // Check arg count
        const auto fogColor = GetAmxString(amx, params[arg_color]);
        auto color = Colors.parse(fogColor);
        if (!color) {
            return false;
        }
        const auto densityPercentage = params[arg_density_percentage];
        return Fogs.add(fogColor, densityPercentage);
    }

    auto HandleFogVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_fog,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = FogVars;

        const int fogId = params[arg_fog];
        const auto fogRef = Fogs[fogId];
        if (!fogRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(FogVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& fog = fogRef->get();
        switch (*var) {
            case vars::Color: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], fog.getColor().c_str(), params[arg_4]);
                } else {
                    auto fogColor = GetAmxString(amx, params[arg_3]);
                    auto color = Colors.parse(fogColor);
                    if (!color) {
                        return false;
                    }
                    fog.setColor(fogColor);
                }
                break;
            }
            case vars::DensityPercentage: {
                if (isGetter) {
                    return fog.getDensityPercentage();
                } else {
                    // check 0..100
                    fog.setDensityPercentage(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_fog_var(Amx* amx, cell* params) -> cell {
        return HandleFogVar(amx, params, true);
    }

    auto set_fog_var(Amx* amx, cell* params) -> cell {
        return HandleFogVar(amx, params, false);
    }

    auto AmxxFog::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_fog",  create_fog},
            {"get_fog_var", get_fog_var},
            {"set_fog_var", set_fog_var},

            {nullptr,       nullptr},
        };
        AddNatives(natives);
    }
}
