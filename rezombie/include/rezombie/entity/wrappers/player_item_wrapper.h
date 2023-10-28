#pragma once

#include "cssdk/public/regamedll.h"

namespace rz
{
    using namespace cssdk;

    class PlayerItem : public PlayerItemBase {
      public:
        auto SetObjectCollisionBox() -> void override;
        auto RemoveThink() -> void;
        auto TouchWeapon(EntityBase* other) -> void;

        auto getOrigin() const -> Vector { return vars->origin; }

        auto setAbsMin(const Vector& absMin) -> void { vars->abs_min = absMin; }

        auto setAbsMax(const Vector& absMax) -> void { vars->abs_max = absMax; }

        auto setFlags(int flags) -> void { vars->flags = flags; }

        auto getFlags() const -> int { return vars->flags; }

        auto setSolid(SolidType type) -> void { vars->solid = type; }
    };
}
