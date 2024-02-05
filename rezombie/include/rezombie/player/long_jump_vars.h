#pragma once

#include "rezombie/player/api/long_jump.h"

namespace rz
{
    enum class LongJumpState : int {
        None,
        Cooldown,
        Ready,
    };

    class LongJumpVars {
        //int id_ = 0;
        LongJumpState state_ = LongJumpState::None;
        float nextStateTime_ = 0.f;
        int force_ = 0;
        int height_ = 0;
        int cooldown_ = 0;
        int cooldownTimer_ = 0;

      public:
        //auto getId() const -> int { return id_; }

        //auto setId(int flashlightId) -> void { id_ = flashlightId; }

        auto getState() const -> LongJumpState { return state_; }

        auto setState(int player, LongJumpState state) -> void;

        auto getNextStateTime() const -> float { return nextStateTime_; }

        auto setNextStateTime(float time) -> void { nextStateTime_ = time; }

        auto getForce() const -> int { return force_; }

        auto setForce(int force) -> void { force_ = force; }

        auto getHeight() const -> int { return height_; }

        auto setHeight(int height) -> void { height_ = height; }

        auto getCooldown() const -> int { return cooldown_; }

        auto setCooldown(int cooldown) -> void { cooldown_ = cooldown; }

        auto getCooldownTimer() const -> int { return cooldownTimer_; }

        auto setCooldownTimer(int timer) -> void { cooldownTimer_ = timer; }

        auto reset(int player) -> void {
            setState(player, LongJumpState::None);
            setNextStateTime(0);
            setForce(0);
            setHeight(0);
            setCooldown(0);
        }
    };
}
