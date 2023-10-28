#pragma once

#include "rezombie/core/object.h"
#include <string>

namespace rz
{
    class Mode : public Object {
        std::string hudColor_{"white"};
        int dropChance_ = 20;
        int minimumPlayers_ = 0;
        int roundTime_ = 0;
        int launchForward_{};

      public:
        Mode(
            std::string handle,
            int launchForward
        ) : Object(std::move(handle)),
            launchForward_(launchForward) {}

        auto getHudColor() const -> const std::string& { return hudColor_; }

        auto setHudColor(std::string hudColor) -> void { hudColor_ = std::move(hudColor); }

        auto getDropChance() const { return dropChance_; }

        auto setDropChance(int dropChance) { dropChance_ = dropChance; }

        auto getMinPlayers() const { return minimumPlayers_; }

        auto setMinPlayers(int minimumPlayers) { minimumPlayers_ = minimumPlayers; }

        auto getRoundTime() const { return roundTime_; }

        auto setRoundTime(int roundTime) { roundTime_ = roundTime; }

        auto getLaunchForward() const { return launchForward_; }

        auto setLaunchForward(int launchForward) { launchForward_ = launchForward; }

        auto executeLaunch(int modeId) const -> bool {
            // no need because consturctor
            if (getLaunchForward() == FORWARD_INVALID) {
                return false;
            }
            ExecuteForward(getLaunchForward(), modeId);
            return true;
        }
    };
}
