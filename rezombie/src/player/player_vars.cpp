#pragma once

#include "rezombie/player/player.h"
#include "rezombie/amxmodx/player.h"

namespace rz::player
{
    auto Player::ResetVars() -> void {
        setClass(0);
        setSubclass(0);
        setProps(0);
        setModel(0);
        setSound(0);
        setMelee(0);
        setNightVision(0);
        ResetExtraJumps();
        ResetLongJump();
    }

    auto Player::ResetExtraJumps() -> void {
        setExtraJumpsCount(0);
        setMaxExtraJumps(0);
    }

    auto Player::ResetLongJump() -> void {
        setLongJumpState(LongJumpState::None);
        setLongJumpNextStateTime(0);
        setLongJumpForce(0);
        setLongJumpHeight(0);
        setLongJumpCooldown(0);
    }

    auto Player::getClass() const -> int {
        return playerVars_.classIndex;
    }

    auto Player::setClass(int index) -> void {
        playerVars_.classIndex = index;
    }

    auto Player::getSubclass() const -> int {
        return playerVars_.subclassIndex;
    }

    auto Player::setSubclass(int index) -> void {
        playerVars_.subclassIndex = index;
    }

    auto Player::getProps() const -> int {
        return playerVars_.propsIndex;
    }

    auto Player::setProps(int index) -> void {
        playerVars_.propsIndex = index;
    }

    auto Player::getModel() const -> int {
        return playerVars_.modelIndex;
    }

    auto Player::setModel(int index) -> void {
        playerVars_.modelIndex = index;
    }

    auto Player::getSound() const -> int {
        return playerVars_.soundIndex;
    }

    auto Player::setSound(int index) -> void {
        playerVars_.soundIndex = index;
    }

    auto Player::getMelee() const -> int {
        return playerVars_.meleeIndex;
    }

    auto Player::setMelee(int index) -> void {
        playerVars_.meleeIndex = index;
    }

    auto Player::getNightVision() const -> int {
        return playerVars_.nightVisionIndex;
    }

    auto Player::setNightVision(int index) -> void {
        playerVars_.nightVisionIndex = index;
    }

    auto Player::getExtraJumpsCount() const -> int {
        return playerVars_.extraJumpsCount;
    }

    auto Player::setExtraJumpsCount(int amount) -> void {
        playerVars_.extraJumpsCount = amount;
    }

    auto Player::getMaxExtraJumps() const -> int {
        return playerVars_.maxExtraJumps;
    }

    auto Player::setMaxExtraJumps(int amount) -> void {
        playerVars_.maxExtraJumps = amount;
    }

    auto Player::getLongJumpState() const -> LongJumpState {
        return playerVars_.longJumpState;
    }

    auto Player::setLongJumpState(LongJumpState state) -> void {
        const auto oldState = playerVars_.longJumpState;
        playerVars_.longJumpState = state;
        if (oldState == state) {
            return;
        }
        amxxPlayer.LongJumpState(*this);
    }

    auto Player::getLongJumpNextStateTime() const -> float {
        return playerVars_.longJumpNextStateTime;
    }

    auto Player::setLongJumpNextStateTime(float nextStateTime) -> void {
        playerVars_.longJumpNextStateTime = nextStateTime;
    }

    auto Player::getLongJumpForce() const -> int {
        return playerVars_.longJumpForce;
    }

    auto Player::setLongJumpForce(int force) -> void {
        playerVars_.longJumpForce = force;
    }

    auto Player::getLongJumpHeight() const -> int {
        return playerVars_.longJumpHeight;
    }

    auto Player::setLongJumpHeight(int height) -> void {
        playerVars_.longJumpHeight = height;
    }

    auto Player::getLongJumpCooldown() const -> float {
        return playerVars_.longJumpCooldown;
    }

    auto Player::setLongJumpCooldown(float cooldown) -> void {
        playerVars_.longJumpCooldown = cooldown;
    }

    auto Player::getPreview() const -> PlayerPreview* {
        return preview_;
    }
}
