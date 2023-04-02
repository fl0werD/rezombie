#include "rezombie/player/player_preview.h"
#include "rezombie/player/player.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz::player
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto previewPrecache() -> void
    {
        g_previewSkeleton_ModelIndex = PrecacheModel(PREVIEW_SKELETON);
        g_previewPlayer_ModelIndex = PrecacheModel(PREVIEW_PLAYER);
        g_previewWeapon_ModelIndex = PrecacheModel(PREVIEW_WEAPON);
    }

    auto PlayerPreview::CreatePreview() -> void
    {
        const auto skeleton = CreateNamedEntity(AllocString("info_target"));
        const auto playerModel = CreateNamedEntity(AllocString("info_target"));
        const auto weaponModel = CreateNamedEntity(AllocString("info_target"));

        skeleton->vars.class_name = AllocString("ent_preview_skeleton");
        // dummy->vars.sequence = 0;
        // dummy->vars.frame_rate = 1.f;
        // dummy->vars.anim_time = g_global_vars->time;
        skeleton->vars.model_index = g_previewSkeleton_ModelIndex;

        playerModel->vars.class_name = AllocString("ent_preview_player");
        playerModel->vars.move_type = MoveTypeEntity::Follow;
        playerModel->vars.aim_entity = skeleton;
        playerModel->vars.model_index = g_previewPlayer_ModelIndex;

        weaponModel->vars.class_name = AllocString("ent_preview_weapon");
        weaponModel->vars.move_type = MoveTypeEntity::Follow;
        weaponModel->vars.aim_entity = skeleton;
        weaponModel->vars.model_index = g_previewWeapon_ModelIndex;
    }

    auto PlayerPreview::RemovePreview() -> void
    {
        skeleton_->vars.flags = FL_KILL_ME;
        playerModel_->vars.flags = FL_KILL_ME;
        weaponModel_->vars.flags = FL_KILL_ME;
    }

    auto PlayerPreview::ChangePlayerModel(int modelIndex) -> void
    {
        playerModel_->vars.model_index = modelIndex;
    }

    auto PlayerPreview::UpdatePosition(Vector gunPosition, Vector& viewAngle) -> void
    {
        const auto origin = gunPosition + g_global_vars->vec_forward * getViewForwardDistance();
        MakeVectors(viewAngle);
        skeleton_->vars.angles = viewAngle;
        skeleton_->vars.angles.y += 180.f;
        SetOrigin(skeleton_, origin);
    }
}
