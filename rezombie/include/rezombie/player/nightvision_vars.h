#pragma once

namespace rz
{
    class NightVisionVars {
        int id_ = 0;
        bool isEnabled_ = false;

      public:
        auto getId() const -> int { return id_; }

        auto setId(int nightVisionId) -> void { id_ = nightVisionId; }

        auto isEnabled() const -> bool { return isEnabled_; }

        auto setEnabled(bool isEnabled) -> void { isEnabled_ = isEnabled; }
    };
}
