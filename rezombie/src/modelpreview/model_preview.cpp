#include "rezombie/modelpreview/model_preview.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto ModelPreview::createEntities() -> void {
        auto entityClassName = AllocString("info_target");
        EntityVars* vars;

        setEntity(PreviewType::ParentModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(PreviewType::ParentModel)->vars;
        vars->class_name = AllocString("preview_parent");
        vars->model_index = -1;
        vars->effects = EF_BRIGHT_LIGHT;

        setEntity(PreviewType::AttachModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(PreviewType::AttachModel)->vars;
        vars->class_name = AllocString("preview_attach");
        vars->model_index = -1;
        vars->move_type = MoveTypeEntity::Follow;
        vars->aim_entity = getEntity(PreviewType::ParentModel);

        setEntity(PreviewType::ExtraAttachModel, UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity(PreviewType::ExtraAttachModel)->vars;
        vars->class_name = AllocString("preview_extra_attach");
        vars->model_index = -1;
        vars->move_type = MoveTypeEntity::Follow;
        vars->aim_entity = getEntity(PreviewType::ParentModel);
    }
}
