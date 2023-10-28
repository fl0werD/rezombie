#pragma once

namespace rz
{
    enum class LongJumpState : int {
        None,
        Cooldown,
        Ready,
    };

    constexpr auto ITEM_LONG_JUMP = "item_longjump";

    class LongJumpVars {
        //int id_ = 0;
        LongJumpState state_ = LongJumpState::None;
        float nextStateTime_ = 0.f;
        int force_ = 0;
        int height_ = 0;
        float cooldown_ = 0.f;

      public:
        //auto getId() const -> int { return id_; }

        //auto setId(int flashlightId) -> void { id_ = flashlightId; }

        auto getState() const -> LongJumpState { return state_; }

        auto setState(LongJumpState state) -> void { state_ = state; }

        auto getNextStateTime() const -> float { return nextStateTime_; }

        auto setNextStateTime(float time) -> void { nextStateTime_ = time; }

        auto getForce() const -> int { return force_; }

        auto setForce(int force) -> void { force_ = force; }

        auto getHeight() const -> int { return height_; }

        auto setHeight(int height) -> void { height_ = height; }

        auto getCooldown() const -> float { return nextStateTime_; }

        auto setCooldown(float cooldown) -> void { nextStateTime_ = cooldown; }

        auto reset() -> void {
            setState(LongJumpState::None);
            setNextStateTime(0);
            setForce(0);
            setHeight(0);
            setCooldown(0);
        }
    };

    /*

    auto Player::setLongJumpState(LongJumpState state) -> void {
        const auto oldState = getLongJumpState();
        playerVars_.longJumpState = state;
        if (oldState == state) {
            return;
        }
        PlayerApi.LongJumpState(*this, toInt(getLongJumpState()));
    }

     */
}
