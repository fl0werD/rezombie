#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxPlayerProps : public AmxxFeature<> {
      public:
        AmxxPlayerProps() : AmxxFeature("player_props") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxPlayerProps PlayerPropsApi;
}
