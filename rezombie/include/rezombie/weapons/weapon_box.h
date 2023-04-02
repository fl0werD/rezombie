#pragma once

#include "rezombie/util.h"
#include <core/regamedll_api.h>
#include <vhooks/vhooks.h>

namespace rz::weapon
{
    using namespace cssdk;
    using namespace vhooks;

    class WeaponBoxVirtuals : public WeaponBox
    {
      private:
        static VirtualHook touch;

      protected:
        auto WeaponBoxTouch(EntityBase* other) -> void;
    };
}
