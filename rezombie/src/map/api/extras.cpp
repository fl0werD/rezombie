#include "rezombie/map/extras.h"
#include "rezombie/map/api/extras.h"
#include <rezombie/entity/extras.h>
#include "rezombie/core/api/amxx_helper.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;

    auto is_map_extra_exists(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_type,
            arg_key,
        };

        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto key = GetAmxString(amx, params[arg_key]);
        return MapExtras.isKeyExists(type, key);
    }

    auto HandleMapExtra(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_type,
            arg_key,
            arg_3,
            arg_4,
        };

        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto* key = GetAmxString(amx, params[arg_key], 0);
        switch (type) {
            case ExtraType::Int: {
                auto& extra = MapExtras.getInts();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid map extra '%s' var (int)", key)
                    return extra[key];
                } else {
                    extra[key] = *Address(amx, params[arg_3]);
                }
                break;
            }
            case ExtraType::Float: {
                auto& extra = MapExtras.getFloats();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid map extra '%s' var (float)", key)
                    return FloatToCell(extra[key]);
                } else {
                    extra[key] = CellToFloat(*Address(amx, params[arg_3]));
                }
                break;
            }
            case ExtraType::String: {
                auto& extra = MapExtras.getStrings();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid map extra '%s' var (string)", key)
                    SetAmxString(amx, params[arg_3], extra[key].c_str(), *Address(amx, params[arg_4]));
                } else {
                    extra[key] = GetAmxString(amx, params[arg_3], 1);
                }
                break;
            }
        }
        return true;
    }

    auto get_map_extra(Amx* amx, cell* params) -> cell {
        return HandleMapExtra(amx, params, true);
    }

    auto set_map_extra(Amx* amx, cell* params) -> cell {
        return HandleMapExtra(amx, params, false);
    }

    auto AmxxMapExtras::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"is_map_extra_exists", is_map_extra_exists},
            {"get_map_extra", get_map_extra},
            {"set_map_extra", set_map_extra},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
