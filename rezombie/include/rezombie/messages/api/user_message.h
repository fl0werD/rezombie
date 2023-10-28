#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxUserMessage : public AmxxFeature<> {
      public:
        AmxxUserMessage() : AmxxFeature("user_message") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxUserMessage UserMessageApi;
}
