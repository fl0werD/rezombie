#include "rezombie/messages/api/engine_message.h"
#include "rezombie/messages/temporary_entity.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto message_beam_follow(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_entity,
            arg_sprite_index,
            arg_life,
            arg_line_width,
            arg_color,
        };

        // Check arg count
        const auto entity = params[arg_entity];
        const auto spriteIndex = params[arg_sprite_index];
        const byte life = params[arg_life];
        const byte lineWidth = params[arg_line_width];
        const auto color = GetAmxString(amx, params[arg_color]);
        const auto colorRef = Colors.parse(color);
        if (!colorRef) {
            return false;
        }
        TE_BeamFollow(entity, spriteIndex, life, lineWidth, *colorRef);
        return true;
    }

    auto AmxxEngineMessage::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"message_beam_follow", message_beam_follow},

            {nullptr,               nullptr},
        };
        AddNatives(natives);
    }
}
