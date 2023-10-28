#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxModelPreview : public AmxxFeature<> {
      public:
        AmxxModelPreview() : AmxxFeature("model_preview") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxModelPreview ModelPreviewApi;
}
