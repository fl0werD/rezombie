#include "rezombie/player/player.h"
#include "rezombie/player/world_preview.h"

namespace rz
{
    auto Player::setWorldPreview(bool isEnabled) -> void {
        auto& preview = getWorldPreviewVars();
        preview.setEnabled(isEnabled);
    }

    auto Player::WorldPreviewUpdate() -> void {
        auto& preview = getWorldPreviewVars();
        if (!preview.isEnabled()) {
            return;
        }
        MakeVectors(getAngles());
        auto previewOrigin = getOrigin() + g_global_vars->vec_forward * WorldPreview.getViewForwardDistance();
        auto previewAngles = getAngles();
        preview.setOrigin(previewOrigin);
        preview.setAngles(previewAngles);
    }
}
