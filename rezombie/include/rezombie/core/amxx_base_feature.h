#pragma once

#include "rezombie/core/base_object.h"
#include "rezombie/amxmodx/utils.h"
#include <amxx/api.h>

namespace rz
{
    class AmxxBaseFeature : public BaseObject
    {
      public:
        explicit AmxxBaseFeature(std::string handle) : BaseObject(std::move(handle))
        {
        }

        virtual auto RegisterForwards() -> void = 0;
        virtual auto RegisterNatives() -> void = 0;
    };
}
