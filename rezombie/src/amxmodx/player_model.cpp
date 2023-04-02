#include "rezombie/amxmodx/player_model.h"
#include "rezombie/modules/player_model.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;

    auto AMX_NATIVE_CALL rz_add_player_model(Amx* amx, cell* params) -> cell
    {
        enum
        {
            arg_count,
            arg_model,
            arg_path,             // file_path, model_path ?
            arg_default_hitboxes, // reoder?
            arg_body,
        };

        const int playerModelIndex = params[arg_model];
        const auto playerModelRef = playerModelModule[playerModelIndex];
        if (!playerModelRef) {
            // Invalid index
            return false;
        }
        auto& playerModel = playerModelRef->get();
        const auto path = GetAmxString(amx, params[arg_path]);
        const bool isDefaultHitboxes = params[arg_default_hitboxes];
        const int body = params[arg_body];
        playerModel.add(path, isDefaultHitboxes, body);
        return true;
    }

    auto AmxxPlayerModel::RegisterNatives() -> void
    {
        static AmxNativeInfo natives[] = {
          {"rz_add_player_model", rz_add_player_model},

          {nullptr,               nullptr            },
        };
        AddNatives(natives);
    }
}
