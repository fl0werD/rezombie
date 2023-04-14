#include "rezombie/player/player.h"

namespace rz::player
{
    auto Player::ExtraJump() -> void {
        if (getFlags() & FL_ON_GROUND) {
            setExtraJumpsCount(0);
            return;
        }
        if (getOldButtons() & IN_JUMP) {
            return;
        }
        if (getExtraJumpsCount() >= getMaxExtraJumps()) {
            return;
        }
        auto& velocity = getVelocity();
        velocity.z = sqrt(2 * 800 * 45.0f);
        setVelocity(velocity);
        setExtraJumpsCount(getExtraJumpsCount() + 1);
    }
}
