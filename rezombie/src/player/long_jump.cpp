#include "rezombie/amxmodx/player.h"
#include "rezombie/player/player.h"
#include <messages/user_message.h>

namespace rz::player {
    using namespace message;

    auto Player::GiveLongJump(int force, int height, float cooldown) -> void {
        setLongJumpState(LongJumpState::Ready);
        setLongJumpForce(force);
        setLongJumpHeight(height);
        setLongJumpCooldown(cooldown);
        sendItemPickup(*this, ITEM_LONG_JUMP);
    }

    auto Player::RemoveLongJump() -> void {
        ResetLongJump();
    }

    auto Player::LongJump() -> void {
        if (getLongJumpState() != LongJumpState::Ready) {
            return;
        }
        if (getFlags() & FL_ON_GROUND) {
            return;
        }
        if (isFrozen()) {
            return;
        }
        if (getWaterLevel() >= 2) {
            return;
        }
        if (isInDuck() && (getFlags() & FL_DUCKING)) {
            return;
        }
        if (!(isInDuck() || (getFlags() & FL_DUCKING))) {
            return;
        }
        if (!((getButton() & IN_DUCK) && getDuckTime() > 0 && getVelocity().Length() > 50)) {
            return;
        }
        MakeVectors(getViewAngle());
        Vector velocity;
        velocity = g_global_vars->vec_forward * static_cast<float>(getLongJumpForce());
        velocity.z = g_global_vars->vec_forward.z * static_cast<float>(getLongJumpHeight());
        setVelocity(velocity);
        if (getLongJumpCooldown() != 0.f) {
            setLongJumpNextStateTime(g_global_vars->time + getLongJumpCooldown());
            setLongJumpState(LongJumpState::Cooldown);
        }
        amxxPlayer.LongJumpActivated(*this);
    }

    auto Player::LongJumpCooldown() -> void {
        if (getLongJumpState() != LongJumpState::Cooldown) {
            return;
        }
        if (getLongJumpNextStateTime() > g_global_vars->time) {
            return;
        }
        setLongJumpNextStateTime(0.f);
        setLongJumpState(LongJumpState::Ready);
    }
}
