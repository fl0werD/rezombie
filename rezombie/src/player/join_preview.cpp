#include "rezombie/player/player.h"

namespace rz
{
    auto Player::setPreview(bool isEnabled) -> void {
        auto& preview = getJoinPreviewVars();
        preview.setEnabled(isEnabled);
        /*if (preview.isEnabled()) {
            SetView(*this, ModelPreview.getCamera());
        } else {
            SetView(*this, *this);
        }*/
    }

    auto Player::PreviewUpdate() -> void {
        auto& cameraVars = getMapCameraVars();
        if (!cameraVars.isEnabled()) {
            return;
        }
        const auto& cameraRef = MapCameras[cameraVars.getCamera()];
        if (!cameraRef) {
            return;
        }
        auto& preview = getJoinPreviewVars();
        if (!preview.isEnabled()) {
            return;
        }
        const auto& camera = cameraRef->get();
        MakeVectors(camera.getAngles());
        auto previewOrigin = camera.getOrigin() + g_global_vars->vec_forward * ModelPreview.getViewForwardDistance();
        auto previewAngles = camera.getAngles();
        previewAngles.y += 180.f;
        preview.setOrigin(previewOrigin);
        if (getButton() & IN_ATTACK) {
            auto angles = previewAngles;
            angles.y -= getAngles().y;
            preview.setAngles(angles);
        } else {
            preview.setAngles(previewAngles);
        }
    }
}
