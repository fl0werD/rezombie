#include "rezombie/player/api/long_jump.h"
#include "rezombie/player/player.h"

namespace rz
{
    auto Player::GiveLongJump(int force, int height, int cooldown) -> void {
        auto& longJump = LongJump();
        longJump.setState(*this, LongJumpState::Ready);
        longJump.setForce(force);
        longJump.setHeight(height);
        longJump.setCooldown(cooldown);
        LongJumpApi.Give(*this);
    }

    auto Player::RemoveLongJump() -> void {
        LongJump().reset(*this);
    }

    auto Player::ActivateLongJump() -> void {
        const auto& longJump = LongJump();
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
        LongJumpApi.Activated(*this);
    }

    auto Player::LongJumpCooldown() -> void {
        auto& longJump = LongJump();
        if (longJump.getState() != LongJumpState::Cooldown) {
            return;
        }
        if (longJump.getNextStateTime() > g_global_vars->time) {
            return;
        }
        longJump.setNextStateTime(g_global_vars->time + 1.f);
        longJump.setCooldownTimer(longJump.getCooldownTimer() - 1);
        if (longJump.getCooldownTimer() > 0) {
            LongJumpApi.CooldownTimer(*this, longJump.getCooldownTimer());
        } else {
            longJump.setNextStateTime(0.f);
            longJump.setState(*this, LongJumpState::Ready);
        }
    }

    auto LongJumpVars::setState(int player, LongJumpState state) -> void {
        const auto oldState = getState();
        state_ = state;
        if (oldState == state) {
            return;
        }
        LongJumpApi.State(player, toInt(state));
    }
}
