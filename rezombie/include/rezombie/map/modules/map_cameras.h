#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/map/camera_data.h"

namespace rz
{
    class MapCamerasModule : public Module<CameraData> {
        Edict* camera_{};

      public:
        MapCamerasModule() : Module("map_cameras") {}

        auto add(Vector origin, Vector angles) -> int {
            return Module::add(new CameraData(origin, angles));
        }

        auto createEntity() -> void;
        auto collect() -> void;

        auto getCamera() -> Edict* { return camera_; }

        auto setCamera(Edict* camera) { camera_ = camera; }
    };

    inline MapCamerasModule MapCameras;
}
