#pragma once

namespace rz
{
    class JumpsVars {
        //int id_ = 0;
        int count_ = 0;
        int maximum_ = 1;

      public:
        //auto getId() const -> int { return id_; }

        //auto setId(int flashlightId) -> void { id_ = flashlightId; }

        auto getCount() const -> int { return count_; }

        auto setCount(int player, int count) -> void;

        auto getMaximum() const -> int { return maximum_; }

        auto setMaximum(int maximum) -> void { maximum_ = maximum; }

        auto reset(int player) -> void {
            setCount(player, 0);
            setMaximum(1);
        }
    };
}
