#include "rezombie/preview/api/join_preview.h"
#include "rezombie/player/players.h"
#include "rezombie/core/api/amxx_helper.h"
#include "amxx/api.h"

namespace rz
{
    using namespace amxx;

    enum class ModelPreviewVars : int {
        IsEnabled,
        ParentModel,
        AttachModel,
        ExtraAttachModel,
    };

    const std::unordered_map<std::string, ModelPreviewVars> ModelPreviewVarsMap = {
        {"enabled",            ModelPreviewVars::IsEnabled},
        {"parent_model",       ModelPreviewVars::ParentModel},
        {"attach_model",       ModelPreviewVars::AttachModel},
        {"extra_attach_model", ModelPreviewVars::ExtraAttachModel},
    };

    auto HandleModelPreviewVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = ModelPreviewVars;

        auto& player = Players[params[arg_player]];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(ModelPreviewVarsMap, key);
        CHECK_VAR_EXISTS("Invalid model preview '%s' var", key)
        auto& preview = player.getJoinPreviewVars();
        switch (*var) {
            case vars::IsEnabled: {
                if (isGetter) {
                    return preview.isEnabled();
                } else {
                    player.setPreview(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::ParentModel: {
                if (isGetter) {
                    return preview.getModel(JoinPreviewType::ParentModel);
                } else {
                    preview.setModel(JoinPreviewType::ParentModel, *Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::AttachModel: {
                if (isGetter) {
                    return preview.getModel(JoinPreviewType::AttachModel);
                } else {
                    preview.setModel(JoinPreviewType::AttachModel, *Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::ExtraAttachModel: {
                if (isGetter) {
                    return preview.getModel(JoinPreviewType::ExtraAttachModel);
                } else {
                    preview.setModel(JoinPreviewType::ExtraAttachModel, *Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_preview_var(Amx* amx, cell* params) -> cell {
        return HandleModelPreviewVar(amx, params, true);
    }

    auto set_preview_var(Amx* amx, cell* params) -> cell {
        return HandleModelPreviewVar(amx, params, false);
    }

    auto AmxxModelPreview::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_preview_var", get_preview_var},
            {"set_preview_var", set_preview_var},

            {nullptr,           nullptr},
        };
        AddNatives(natives);
    }
}
