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

        const auto name = GetAmxString(amx, params[arg_name]);
        const auto colors = Address(amx, params[arg_color]);
        const auto color = Color(
            toUChar(colors[0]),
            toUChar(colors[1]),
            toUChar(colors[2]),
            toUChar(colors[3])
        );
        Colors.add(name, color);
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
