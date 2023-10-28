#include "rezombie/map/api/environment.h"
#include "rezombie/map/environment.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>
#include <unordered_map>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class EnvironmentVars : int {
        SkyName,
        Light,
        Fog,
        WeatherType,
    };

    const std::unordered_map<std::string, EnvironmentVars> EnvironmentVarsMap = {
        {"sky_name",     EnvironmentVars::SkyName},
        {"light",        EnvironmentVars::Light},
        {"fog",          EnvironmentVars::Fog},
        {"weather_type", EnvironmentVars::WeatherType},
    };

    auto HandleEnvironmentVar(Amx* amx, cell* params, bool isGetter, bool isOverride) -> cell {
        enum {
            arg_count,
            arg_var,
            arg_2,
            arg_3,
        };

        using vars = EnvironmentVars;

        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(EnvironmentVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        // TODO why?
        auto& env = Environment.getEnvironment(isOverride);
        switch (*var) {
            case vars::SkyName: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_2], Environment.getSkyName().c_str(), params[arg_3]);
                } else {
                    // only in precache
                    Environment.setSkyName(GetAmxString(amx, params[arg_2]));
                }
                break;
            }
            case vars::Light: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_2], env.getLight().c_str(), params[arg_3]);
                } else {
                    Environment.setLight(GetAmxString(amx, params[arg_2]), isOverride);
                }
                break;
            }
            case vars::Fog: {
                if (isGetter) {
                    return env.getFog();
                } else {
                    Environment.setFog(*Address(amx, params[arg_2]), isOverride);
                }
                break;
            }
            case vars::WeatherType: {
                if (isGetter) {
                    return toInt(env.getWeatherType());
                } else {
                    Environment.setWeatherType(static_cast<WeatherType>(*Address(amx, params[arg_2])), isOverride);
                }
                break;
            }
        }
        return true;
    }

    auto get_env_var(Amx* amx, cell* params) -> cell {
        return HandleEnvironmentVar(amx, params, true, false);
    }

    auto set_env_var(Amx* amx, cell* params) -> cell {
        return HandleEnvironmentVar(amx, params, false, false);
    }

    auto get_override_env_var(Amx* amx, cell* params) -> cell {
        return HandleEnvironmentVar(amx, params, true, true);
    }

    auto set_override_env_var(Amx* amx, cell* params) -> cell {
        return HandleEnvironmentVar(amx, params, false, true);
    }

    auto reset_env(Amx* amx, cell* params) -> cell {
        Environment.reset();
        return true;
    }

    auto AmxxEnvironment::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_env_var",          get_env_var},
            {"set_env_var",          set_env_var},
            {"get_override_env_var", get_override_env_var},
            {"set_override_env_var", set_override_env_var},
            {"reset_env",            reset_env},

            {nullptr,                nullptr},
        };
        AddNatives(natives);
    }
}
