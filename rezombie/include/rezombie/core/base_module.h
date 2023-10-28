#pragma once

#include "rezombie/core/object.h"

namespace rz
{
    class BaseModule : public Object {
      public:
        explicit BaseModule(std::string handle) : Object(std::move(handle)) {}

        virtual auto clear() -> void = 0;
        virtual auto precache() -> void = 0;
    };
}
