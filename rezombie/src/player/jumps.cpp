#include "rezombie/player/player.h"
#include "rezombie/player/api/jumps.h"

namespace rz
{
    auto Player::JumpThink() -> void {
        if (!(getFlags() & FL_ON_GROUND) || !getGroundEntity()) {
            return;
        }
        Jumps().setCount(*this, 0);
    }

    auto Player::Jump() -> void {
        if (getFlags() & FL_WATER_JUMP) {
            return;
        }
        if (getWaterLevel() >= 2) {
            return;
        }
        if (!(getButtonPressed() & IN_JUMP)) {
            return;
        }
        auto& jumps = Jumps();
        if (jumps.getCount() >= jumps.getMaximum() && jumps.getMaximum() != -1) {
            return;
        }
        jumps.setCount(*this, jumps.getCount() + 1);
    }

    auto JumpsVars::setCount(const int player, const int count) -> void {
        const auto oldCount = getCount();
        count_ = count;
        if (oldCount == count) {
            return;
        }
        JumpsApi.Jump(player, count);
    }
}
