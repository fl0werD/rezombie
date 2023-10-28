#include "rezombie/player/modules/player_class.h"
#include "rezombie/models/api/models_pack.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;

    auto models_pack_add_model(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_models_pack,
            arg_model,
        };

        const auto modelsPackRef = ModelsPack[params[arg_models_pack]];
        if (!modelsPackRef) {
            return false;
        }
        auto& modelsPack = modelsPackRef->get();
        const auto modelId = params[arg_model];
        modelsPack.add(modelId);
        return true;
    }

    auto AmxxModelsPack::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"models_pack_add_model", models_pack_add_model},

            {nullptr,                 nullptr},
        };
        AddNatives(natives);
    }
}
