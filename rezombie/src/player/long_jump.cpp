#include "rezombie/player/api/player.h"
#include "rezombie/player/player.h"
#include "rezombie/messages/user_message.h"

namespace rz
{
    auto Player::GiveLongJump(int force, int height, float cooldown) -> void {
        auto& longJump = getLongJump();
        longJump.setState(LongJumpState::Ready);
        longJump.setForce(force);
        longJump.setHeight(height);
        longJump.setCooldown(cooldown);
        sendItemPickup(*this, ITEM_LONG_JUMP);
    }

    auto Player::RemoveLongJump() -> void {
        getLongJump().reset();
    }

    auto Player::LongJump() -> void {
        auto& longJump = getLongJump();
        if (longJump.getState() != LongJumpState::Ready) {
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
        Vector velocity = g_global_vars->vec_forward * static_cast<float>(longJump.getForce());
        velocity.z = g_global_vars->vec_forward.z * static_cast<float>(longJump.getHeight());
        setVelocity(velocity);
        if (longJump.getCooldown() != 0.f) {
            longJump.setNextStateTime(g_global_vars->time + longJump.getCooldown());
            longJump.setState(LongJumpState::Cooldown);
        }
        PlayerApi.LongJumpActivated(*this);
    }

    auto Player::LongJumpCooldown() -> void {
        auto& longJump = getLongJump();
        if (longJump.getState() != LongJumpState::Cooldown) {
            return;
        }
        if (longJump.getNextStateTime() > g_global_vars->time) {
            return;
        }
        longJump.setNextStateTime(0.f);
        longJump.setState(LongJumpState::Ready);
    }
}
