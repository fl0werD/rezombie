#pragma once

#include "rezombie/entity/map/fog.h"
#include "rezombie/messages/weather_type.h"
#include <string>

namespace rz
{
    class EnvironmentData {
        std::string skyName_{};
        std::string light_{};
        int fogId_{};
        WeatherType weatherType_ = WeatherType::None;

      public:
        auto getSkyName() -> const std::string& { return skyName_; }

        auto setSkyName(const std::string& skyName) -> void { skyName_ = skyName; }

        auto getLight() -> const std::string& { return light_; }

        auto setLight(const std::string& light) -> void { light_ = light; }

        auto getFog() -> int { return fogId_; }

        auto setFog(int fogId) -> void { fogId_ = fogId; }

        auto getWeatherType() -> WeatherType { return weatherType_; }

        auto setWeatherType(WeatherType weatherType) -> void { weatherType_ = weatherType; }
    };
}
