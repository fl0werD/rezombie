#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxEngineMessage : public AmxxFeature<> {
      public:
        AmxxEngineMessage() : AmxxFeature("engine_message") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxEngineMessage EngineMessageApi;
}
