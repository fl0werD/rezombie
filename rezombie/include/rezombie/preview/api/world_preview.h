#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxWorldPreview : public AmxxFeature<> {
      public:
        AmxxWorldPreview() : AmxxFeature("world_preview") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxWorldPreview WorldPreviewApi;
}
