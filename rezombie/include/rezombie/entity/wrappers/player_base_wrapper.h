#pragma once

#include "cssdk/public/regamedll.h"

namespace rz
{
    using namespace cssdk;

    class PlayerBaseWrapper : public PlayerBase {
      public:
        auto PlayerDeathThink() -> void;
        auto HasWeapons() -> bool;
    };
}
