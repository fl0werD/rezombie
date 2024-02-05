#include "rezombie/preview/join_preview.h"
#include "metamod/engine.h"
#include "metamod/gamedll.h"

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto ModelPreview::createEntities() -> void {
        auto entityClassName = AllocString("info_target");
        EntityVars* vars;

        setEntity(JoinPreviewType::ParentModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(JoinPreviewType::ParentModel)->vars;
        vars->class_name = AllocString("preview_parent");
        vars->model_index = -1;
        vars->effects = EF_BRIGHT_LIGHT;

        setEntity(JoinPreviewType::AttachModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(JoinPreviewType::AttachModel)->vars;
        vars->class_name = AllocString("preview_attach");
        vars->model_index = -1;
        vars->move_type = MoveTypeEntity::Follow;
        vars->aim_entity = getEntity(JoinPreviewType::ParentModel);

        setEntity(JoinPreviewType::ExtraAttachModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(JoinPreviewType::ExtraAttachModel)->vars;
        vars->class_name = AllocString("preview_extra_attach");
        vars->model_index = -1;
        vars->move_type = MoveTypeEntity::Follow;
        vars->aim_entity = getEntity(JoinPreviewType::ParentModel);
    }
}
