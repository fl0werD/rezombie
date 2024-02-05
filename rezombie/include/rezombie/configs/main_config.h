#pragma once
#include <string>
#include <vector>

namespace rz
{
    class MainConfig {
        protected:
            std::string serverBrowserInfo_ = "ReZombie";
            std::string skyName_ = "space";
            int warmupTime_ = 40;
            int prepareTime_ = 20;
            int roundTime_ = 180;
            std::vector<std::string> uselessEntities_ = {
                "func_bomb_target",
                "info_bomb_target",
                "info_vip_start",
                "func_vip_safetyzone",
                "func_escapezone",
                "func_hostage_rescue",
                "info_hostage_rescue",
                "hostage_entity",
                "armoury_entity",
                "player_weaponstrip",
                "game_player_equip",
                "env_fog",
                "env_rain",
                "env_snow",
                "monster_scientist"
            };

        public:
            auto load() -> void;

            auto getServerBrowswerInfo() const -> const std::string& { return serverBrowserInfo_; }
            auto setServerBrowswerInfo(const std::string& browserInfo) -> void { serverBrowserInfo_ = browserInfo; }
            auto getSkyName() const -> const std::string& { return skyName_; }
            auto setSkyName(const std::string& skyName) -> void { skyName_ = skyName; }
            auto getWarmupTime() const -> int { return warmupTime_; }
            auto setWarmupTime(int warmupTime) -> void { warmupTime_ = warmupTime; }
            auto getPrepareTime() const -> int { return prepareTime_; }
            auto setPrepareTime(int prepareTime) -> void { prepareTime_ = prepareTime; }
            auto getRoundTime() const -> int { return roundTime_; }
            auto setRoundTime(int roundTime) -> void { roundTime_ = roundTime; }
            auto getUselessEntities() const -> const std::vector<std::string>& { return uselessEntities_; }
    };

    inline MainConfig MainConfig;
}
