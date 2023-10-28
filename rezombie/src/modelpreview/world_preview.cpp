#include "rezombie/player/world_preview.h"
#include <metamod/engine.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto WorldPreview::createEntity() -> void {
        auto entityClassName = AllocString("info_target");
        EntityVars* vars;

        setEntity(UTIL_CreateNamedEntity(entityClassName));
        vars = &getEntity()->vars;
        vars->class_name = AllocString("world_preview");
        vars->model_index = -1;
    }
}
