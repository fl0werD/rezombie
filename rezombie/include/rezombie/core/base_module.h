#pragma once

#include "rezombie/core/base_object.h"

namespace rz
{
    class BaseModule : public BaseObject {
      public:
        explicit BaseModule(std::string handle) : BaseObject(std::move(handle)) {}

        virtual auto clear() -> void = 0;
        virtual auto precache() -> void = 0;
    };
}
