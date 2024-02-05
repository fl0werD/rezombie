#pragma once

#include "rezombie/core/object.h"
#include <string>

namespace rz
{
    enum class RespawnType {
        Off,
        ToHumansTeam,
        ToZombiesTeam,
        Balance,
    };

    class Mode : public Object {
        std::string description_;
        std::string noticeMessage_;
        std::string color_{"white"};
        int dropChance_ = 20;
        int minimumPlayers_ = 0;
        int roundTime_ = 0;
        bool isSupportTarget_{};
        RespawnType respawnType_ = RespawnType::Off;
        std::vector<std::string> ambienceSounds_{};
        int launchForward_{};

      public:
        Mode(
            std::string handle,
            int launchForward,
            bool isSupportTarget
        ) : Object(std::move(handle)),
            launchForward_(launchForward),
            isSupportTarget_(isSupportTarget) {}

        auto getDescription() const -> const std::string& { return description_; }

        auto setDescription(std::string description) -> void { description_ = std::move(description); }

        auto getNoticeMessage() const -> const std::string& { return noticeMessage_; }

        auto setNoticeMessage(std::string noticeMessage) -> void { noticeMessage_ = std::move(noticeMessage); }

        auto getColor() const -> const std::string& { return color_; }

        auto setColor(std::string color) -> void { color_ = std::move(color); }

        auto getDropChance() const -> int { return dropChance_; }

        auto setDropChance(int dropChance) -> void { dropChance_ = dropChance; }

        auto getMinPlayers() const -> int { return minimumPlayers_; }

        auto setMinPlayers(int minimumPlayers) -> void { minimumPlayers_ = minimumPlayers; }

        auto getRoundTime() const -> int { return roundTime_; }

        auto setRoundTime(int roundTime) -> void { roundTime_ = roundTime; }

        auto isSupportTarget() const -> bool { return isSupportTarget_; }

        auto setSupportTarget(bool isSupportTarget) -> void { isSupportTarget_ = isSupportTarget; }

        auto getRespawn() const -> RespawnType { return respawnType_; }

        auto setRespawn(const RespawnType respawnType) -> void { respawnType_ = respawnType; }

        auto addAmbienceSound(const std::string& path) -> void {
            ambienceSounds_.emplace_back(path);
        }

        auto getRandomAmbienceSound() const -> const std::string& {
            // if (sounds_[toInt(soundType)].empty()) {
            //     return nullptr;
            // }
            return ambienceSounds_[RandomLong(0, ambienceSounds_.size() - 1)];
        }

        auto getLaunchForward() const -> int { return launchForward_; }

        auto setLaunchForward(int launchForward) -> void { launchForward_ = launchForward; }

        auto executeLaunch(int target = 0) const -> int {
            return ExecuteForward(getLaunchForward(), target);
        }
    };
}
