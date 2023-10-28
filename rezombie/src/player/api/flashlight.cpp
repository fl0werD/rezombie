#include "rezombie/player/api/flashlight.h"
#include "rezombie/player/modules/flashlight.h"
#include "rezombie/core/api/amxx_helper.h"

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class FlashlightVars : int {
        Color,
        Size,
        Distance,
        DrainTime,
        ChargeTime,
    };

    const std::unordered_map<std::string, FlashlightVars> FlashlightVarsMap = {
        {"color",       FlashlightVars::Color},
        {"size",        FlashlightVars::Size},
        {"distance",    FlashlightVars::Distance},
        {"drain_time",  FlashlightVars::DrainTime},
        {"charge_time", FlashlightVars::ChargeTime},
    };

    auto create_flashlight(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_color,
            arg_size,
            arg_drain_time,
            arg_charge_time,
        };

        // Check arg count
        const auto color = GetAmxString(amx, params[arg_color]);
        const int size = params[arg_size];
        const auto drainTime = static_cast<float>(params[arg_drain_time]);
        const auto chargeTime = static_cast<float>(params[arg_charge_time]);
        const auto flashlightId = Flashlights.add(color, size, drainTime / 100.f, chargeTime / 100.f);
        return flashlightId;
    }

    auto HandleFlashlightVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_flashlight,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = FlashlightVars;

        const int flashlightId = params[arg_flashlight];
        const auto flashlightRef = Flashlights[flashlightId];
        if (!flashlightRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(FlashlightVarsMap, key);
        CHECK_VAR_EXISTS("Invalid getFlashlight '%s' var", key)
        auto& flashlight = flashlightRef->get();
        switch (*var) {
            case vars::Color: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], flashlight.getColor().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // check valideate
                    flashlight.setColor(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Size: {
                if (isGetter) {
                    return flashlight.getSize();
                } else {
                    // check size
                    flashlight.setSize(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Distance: {
                if (isGetter) {
                    return flashlight.getDistance();
                } else {
                    // if size > 0
                    flashlight.setDistance(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::DrainTime: {
                if (isGetter) {
                    return FloatToCell(flashlight.getDrainTime());
                } else {
                    // check validate
                    flashlight.setDrainTime(static_cast<float>(*Address(amx, params[arg_3])) / 100.f);
                }
                break;
            }
            case vars::ChargeTime: {
                if (isGetter) {
                    return FloatToCell(flashlight.getChargeTime());
                } else {
                    // check validate
                    flashlight.setChargeTime(static_cast<float>(*Address(amx, params[arg_3])) / 100.f);
                }
                break;
            }
        }
        return true;
    }

    auto get_flashlight_var(Amx* amx, cell* params) -> cell {
        return HandleFlashlightVar(amx, params, true);
    }

    auto set_flashlight_var(Amx* amx, cell* params) -> cell {
        return HandleFlashlightVar(amx, params, false);
    }

    auto AmxxFlashlight::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_flashlight",  create_flashlight},
            {"get_flashlight_var", get_flashlight_var},
            {"set_flashlight_var", set_flashlight_var},

            {nullptr,              nullptr},
        };
        AddNatives(natives);
    }
}
