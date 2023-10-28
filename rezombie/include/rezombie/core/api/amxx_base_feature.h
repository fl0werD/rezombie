#pragma once

#include "rezombie/core/object.h"
#include <amxx/api.h>

namespace rz
{
    class AmxxBaseFeature : public Object {
      public:
        explicit AmxxBaseFeature(std::string handle) : Object(std::move(handle)) {}

        virtual auto registerForwards() -> void = 0;
        virtual auto registerNatives() const -> void = 0;
    };
}
