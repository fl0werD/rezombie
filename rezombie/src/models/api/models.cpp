#include "rezombie/models/api/model.h"
#include "rezombie/models/modules/models.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class ModelVars : int {
        Handle,
        Path,
        Body,
        Skin,
        PrecacheId,
    };

    // TODO: string_view?
    const std::unordered_map<std::string, ModelVars> ModelVarsMap = {
        {"handle",      ModelVars::Handle},
        {"path",        ModelVars::Path},
        {"body",        ModelVars::Body},
        {"skin",        ModelVars::Skin},
        {"precache_id", ModelVars::PrecacheId},
    };

    auto create_model(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_path,
            arg_body,
            arg_skin,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle], 0);
        const auto path = GetAmxString(amx, params[arg_path], 1);
        const int body = params[arg_body];
        const int skin = params[arg_skin];
        const auto modelId = Models.add(handle, path, body, skin);
        return modelId;
    }

    auto HandleModelVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_model,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = ModelVars;

        const auto modelRef = Models[params[arg_model]];
        if (!modelRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(ModelVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& model = modelRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], model.getHandle().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Path: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], model.getPath().c_str(), *Address(amx, params[arg_4]));
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Body: {
                if (isGetter) {
                    return model.getBody();
                } else {
                    model.setBody(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Skin: {
                if (isGetter) {
                    return model.getSkin();
                } else {
                    model.setSkin(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::PrecacheId: {
                if (isGetter) {
                    return model.getPrecacheId();
                } else {
                    // Invalid set vars
                }
                break;
            }
        }
        return true;
    }

    auto get_model_var(Amx* amx, cell* params) -> cell {
        return HandleModelVar(amx, params, true);
    }

    auto set_model_var(Amx* amx, cell* params) -> cell {
        return HandleModelVar(amx, params, false);
    }

    auto AmxxModels::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_model",  create_model},
            {"get_model_var", get_model_var},
            {"set_model_var", set_model_var},

            {nullptr,         nullptr},
        };
        AddNatives(natives);
    }
}
