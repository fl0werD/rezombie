#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    enum class ModesForward : int {
        ModeStart,
        MAX_FORWARDS,
    };

    class AmxxModes : public AmxxFeature<ModesForward, ModesForward::MAX_FORWARDS> {
      public:
        AmxxModes() : AmxxFeature("modes") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto ModeStart(int modeId) -> void;
    };

    inline AmxxModes ModesApi;
}
