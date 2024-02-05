#include "rezombie/preview/api/world_preview.h"
#include "rezombie/core/api/amxx_helper.h"
#include "rezombie/player/players.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class RzWorldPreviewVars : int {
        IsEnabled,
        Model,
        MaxDistance,
    };

    const std::unordered_map<std::string, RzWorldPreviewVars> WorldPreviewVarsMap = {
        {"enabled",      RzWorldPreviewVars::IsEnabled},
        {"model",        RzWorldPreviewVars::Model},
        {"max_distance", RzWorldPreviewVars::MaxDistance},
    };

    auto HandleWorldPreviewVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
        };

        using vars = RzWorldPreviewVars;

        const int playerId = params[arg_player];
        auto& player = Players[playerId];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(WorldPreviewVarsMap, key);
        CHECK_VAR_EXISTS("Invalid world preview '%s' var", key)
        auto& preview = player.getWorldPreviewVars();
        switch (*var) {
            case vars::IsEnabled: {
                if (isGetter) {
                    return preview.isEnabled();
                } else {
                    preview.setEnabled(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Model: {
                if (isGetter) {
                    return preview.getModel();
                } else {
                    preview.setModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::MaxDistance: {
                if (isGetter) {
                    return preview.getMaxDistance();
                } else {
                    preview.setMaxDistance(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_world_preview_var(Amx* amx, cell* params) -> cell {
        return HandleWorldPreviewVar(amx, params, true);
    }

    auto set_world_preview_var(Amx* amx, cell* params) -> cell {
        return HandleWorldPreviewVar(amx, params, false);
    }

    auto AmxxWorldPreview::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_world_preview_var", get_world_preview_var},
            {"set_world_preview_var", set_world_preview_var},

            {nullptr,                 nullptr},
        };
        AddNatives(natives);
    }
}
