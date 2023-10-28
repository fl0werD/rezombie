#include "rezombie/core/module.h"
#include "rezombie/models/modules/models.h"

namespace rz
{
    auto setModel(EntityVars* vars, int modelId) -> bool {
        const auto& modelRef = Models[modelId];
        if (!modelRef) {
            return false;
        }
        const auto& model = modelRef->get();
        vars->model_index = model.getPrecacheId();
        vars->body = model.getBody();
        vars->skin = model.getSkin();
        return true;
    }

    auto setModel(EntityState* state, int modelId) -> bool {
        const auto& modelRef = Models[modelId];
        if (!modelRef) {
            return false;
        }
        const auto& model = modelRef->get();
        state->model_index = model.getPrecacheId();
        state->body = model.getBody();
        state->skin = static_cast<short>(model.getSkin());
        return true;
    }
}
