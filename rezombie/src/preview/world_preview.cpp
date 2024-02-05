#include "rezombie/player/player.h"
#include "rezombie/preview/world_preview.h"

namespace rz
{
    auto WorldPreview::createEntity() -> void {
        auto entityClassName = AllocString("info_target");
        EntityVars* vars;

        setEntity(UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity()->vars;
        vars->class_name = AllocString("world_preview");
        vars->model_index = -1;
        vars->move_type = MoveTypeEntity::NoClip;
    }

    auto Player::setWorldPreview(bool isEnabled) -> void {
        auto& preview = getWorldPreviewVars();
        preview.setEnabled(isEnabled);
    }

    auto Player::WorldPreviewUpdate() -> void {
        auto& preview = getWorldPreviewVars();
        if (!preview.isEnabled()) {
            return;
        }
        MakeVectors(getViewAngle());
        auto angles = getAngles();
        auto end = getGunPosition() + g_global_vars->vec_forward * static_cast<float>(preview.getMaxDistance());
        TraceResult trace;
        TraceLine(getGunPosition(), end, TR_IGNORE_MONSTERS, *this, &trace);
        if (trace.fraction != 1.f) {
            end = trace.end_position;
            VecToAngles(trace.plane_normal, angles);
        }
        preview.setOrigin(end);
        preview.setAngles(angles);
    }
}
