#include "rezombie/map/modules/map_cameras.h"
#include "rezombie/map/map_spawns.h"
#include "rezombie/main/util.h"
#include <metamod/engine.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto MapCamerasModule::createEntity() -> void {
        auto entityClassName = AllocString("info_target");
        EntityVars* vars;

        setCamera(UTIL_CreateNamedEntity(entityClassName));
        vars = &getCamera()->vars;
        vars->class_name = AllocString("map_camera");
        vars->model_index = -1;
    }

    auto MapCamerasModule::collect() -> void {
        ForEachEntity(FindEntityType::ClassName, "trigger_camera", [&](const auto camera) {
            const auto& target = FindEntity(nullptr, FindEntityType::TargetName, SzFromIndex(camera->vars->target));
            if (!target) {
                return;
            }
            Vector origin = camera->vars->origin;
            Vector direction = (target->vars->origin - origin).Normalize();
            Vector angles;
            VecToAngles(direction, angles);
            angles.x = -angles.x;
            add(origin, angles);
            camera->vars->flags = FL_KILL_ME;
        });
        if (count() >= 1) {
            return;
        }
        const auto& spawn = MapSpawns[0];
        add(spawn.getOrigin(), spawn.getAngles());
    }
}
