#pragma once

#include "rezombie/player/player.h"

namespace rz
{
    auto Player::ResetVars(bool isInit) -> void {
        setClass(0);
        setSubclass(0);
        setProps(0);
        setModel(0);
        setSounds(0);
        setMelee(0);
        //setNightVision(0);
        //setNightVisionEnabled(false);
        Jumps().reset(*this);
        LongJump().reset(*this);
        setFreezeEndTime(0.f);
        if (isInit) {
            ResetSubclasses();
            Extras().reset();
        }
    }

    auto Player::ResetSubclasses() -> void {
        playerVars_.subclasses.clear();
    }

    auto Player::getClass() const -> int { return playerVars_.classId; }

    auto Player::setClass(int index) -> void { playerVars_.classId = index; }

    auto Player::getSubclass() const -> int {
        return playerVars_.subclassId;
    }

    auto Player::setSubclass(int subclassId) -> void {
        playerVars_.subclassId = subclassId;
    }

    auto Player::getKeepSubclass(int classId) const -> int {
        const auto& subclassId = getMapValue(playerVars_.subclasses, classId);
        if (!subclassId) {
            return 0;
        }
        return *subclassId;
    }

    auto Player::setKeepSubclass(int classId, int subclassId) -> void {
        playerVars_.subclasses[classId] = subclassId;
    }

    auto Player::getProps() const -> int { return playerVars_.propsId; }

    auto Player::setProps(int index) -> void { playerVars_.propsId = index; }

    auto Player::getModel() const -> int { return playerVars_.modelId; }

    auto Player::setModel(int index) -> void { playerVars_.modelId = index; }

    auto Player::getSounds() const -> int { return playerVars_.soundsId; }

    auto Player::setSounds(int index) -> void { playerVars_.soundsId = index; }

    auto Player::getMelee() const -> int { return playerVars_.meleeId; }

    auto Player::setMelee(int index) -> void { playerVars_.meleeId = index; }

    auto Player::getFreezeEndTime() const -> float { return playerVars_.freezeEndTime; }

    auto Player::setFreezeEndTime(float freezeEndTime) -> void { playerVars_.freezeEndTime = freezeEndTime; }
}
