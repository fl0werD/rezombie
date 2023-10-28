#include "rezombie/player/player.h"

namespace rz
{
    auto Player::ExtraJump() -> void {
        auto& extraJump = getExtraJump();
        if (getFlags() & FL_ON_GROUND) {
            extraJump.setCount(0);
            return;
        }
        if (getOldButtons() & IN_JUMP) {
            return;
        }
        if (extraJump.getCount() >= extraJump.getMaximum()) {
            return;
        }
        auto& velocity = getVelocity();
        velocity.z = sqrt(2 * 800 * 45.0f);
        setVelocity(velocity);
        extraJump.setCount(extraJump.getCount() + 1);
    }
}
