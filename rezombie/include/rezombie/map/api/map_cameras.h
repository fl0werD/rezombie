#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxMapCameras: public AmxxFeature<> {
      public:
        AmxxMapCameras() : AmxxFeature("map_cameras") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxMapCameras MapCamerasApi;
}
