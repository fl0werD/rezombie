#include "rezombie/colors/colors.h"
#include "rezombie/colors/api/colors.h"
#include "rezombie/main/util.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto create_color(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_name,
            arg_color,
        };

        const auto name = GetAmxString(amx, params[arg_name], 0);
        const auto color = GetAmxString(amx, params[arg_color], 1);
        const auto colorRef = Colors.parse(color);
        if (!colorRef) {
            return false;
        }
        Colors.add(name, *colorRef);
        return true;
    }

    auto AmxxColors::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_color", create_color},

            {nullptr,        nullptr},
        };
        AddNatives(natives);
    }
}
