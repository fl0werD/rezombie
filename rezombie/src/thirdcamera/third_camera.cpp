#include "rezombie/thirdcamera/third_camera.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto ThirdCamera::createEntity() -> void {
        auto entityClassName = AllocString("info_target");

        setCamera(UTIL_CreateNamedEntity(entityClassName));
        auto vars = &getCamera()->vars;
        vars->class_name = AllocString("third_camera");
        vars->model_index = PrecacheModel(AllocString("models/w_hegrenade.mdl"));//-1;
        vars->move_type = MoveTypeEntity::NoClip;
    }
}
