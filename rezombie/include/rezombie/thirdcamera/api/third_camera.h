#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxThirdCamera : public AmxxFeature<> {
      public:
        AmxxThirdCamera() : AmxxFeature("third_camera") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxThirdCamera ThirdCameraApi;
}
