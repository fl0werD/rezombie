#include "rezombie/thirdcamera/third_camera.h"
#include "rezombie/thirdcamera/api/third_camera.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>
#include <core/rehlds_api.h>

namespace rz
{
    using namespace core;
    using namespace amxx;

    /*enum class ModelPreviewVars : int {
        ParentModel,
        AttachModel,
        ExtraAttachModel,
    };

    const std::unordered_map<std::string, ModelPreviewVars> ModelPreviewVarsMap = {
        {"skeleton_model",     ModelPreviewVars::ParentModel},
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

        using var = ModelPreviewVars;

        auto& player = Players[params[arg_player]];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& value = getMapValue(ModelPreviewVarsMap, key);
        if (!value) {
            // Invalid index
            return false;
        }
        auto& preview = player.getPreview();
        switch (*value) {
            case var::ParentModel: {
                if (isGetter) {
                    return preview->getModel(JoinPreviewType::ParentModel);
                } else {
                    preview->setModel(JoinPreviewType::ParentModel, *Address(amx, params[arg_3]));
                }
                break;
            }
            case var::AttachModel: {
                if (isGetter) {
                    return preview->getModel(JoinPreviewType::AttachModel);
                } else {
                    preview->setModel(JoinPreviewType::AttachModel, *Address(amx, params[arg_3]));
                }
                break;
            }
            case var::ExtraAttachModel: {
                if (isGetter) {
                    return preview->getModel(JoinPreviewType::ExtraAttachModel);
                } else {
                    preview->setModel(JoinPreviewType::ExtraAttachModel, *Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_get_camera_var(Amx* amx, cell* params) -> cell {
        return HandleModelPreviewVar(amx, params, true);
    }

    auto AMX_NATIVE_CALL rz_set_camera_var(Amx* amx, cell* params) -> cell {
        return HandleModelPreviewVar(amx, params, false);
    }*/

    auto is_third_camera(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerId = params[arg_player];
        auto client = rehlds_api::ServerStatic()->GetClient(playerId - 1);
        if (client == nullptr || !(client->active || client->spawned || client->connected)) {
            LogError(amx, AmxError::Native, "%s: player %i is not connected", __func__, playerId);
            return false;
        }
        return (client->view_entity == ThirdCamera.getCamera());
    }

    auto set_third_camera(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_enabled,
        };

        auto& player = Players[params[arg_player]];
        const bool isEnabled = params[arg_enabled];
        player.setThirdCamera(isEnabled);
        return true;
    }

    auto AmxxThirdCamera::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"is_third_camera",  is_third_camera},
            {"set_third_camera", set_third_camera},
            //{"rz_get_camera_var", rz_get_camera_var},
            //{"rz_set_camera_var", rz_set_camera_var},

            {nullptr,            nullptr},
        };
        AddNatives(natives);
    }
}
