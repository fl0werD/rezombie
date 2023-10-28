#pragma once

namespace rz
{
    // add forward for effects maybe

    class ExtraJumpVars {
        //int id_ = 0;
        int count_ = 0;
        int maximum_ = 0;

      public:
        //auto getId() const -> int { return id_; }

        //auto setId(int flashlightId) -> void { id_ = flashlightId; }

        auto getCount() const -> int { return count_; }

        auto setCount(int count) -> void { count_ = count; }

        auto getMaximum() const -> int { return maximum_; }

        auto setMaximum(int maximum) -> void { maximum_ = maximum; }

        auto reset() -> void {
            setCount(0);
            setMaximum(0);
        }
    };
}
