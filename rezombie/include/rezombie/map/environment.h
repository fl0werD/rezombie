#pragma once

#include "rezombie/entity/map/environment_data.h"
#include <string>

namespace rz
{
    class Environment {
        EnvironmentData source_;
        EnvironmentData override_;
        /*std::string skyName_{};
        std::string light_{};
        std::string fogColor_{};
        int fogDensityPercentage_{};
        WeatherType weatherType_ = WeatherType::None;*/

      public:
        auto precache() -> void;

        auto reset() -> void;

        auto getEnvironment(bool isOverride) -> EnvironmentData& { return (isOverride ? override_ : source_); }

        auto getSkyName() -> const std::string& { return source_.getSkyName(); }

        auto setSkyName(const std::string& skyName) -> void { source_.setSkyName(skyName); }

        auto getLight() -> const std::string&;

        auto setLight(const std::string& light, bool isOverride) -> void;

        auto getFog() -> int;

        auto setFog(int fogId, bool isOverride) -> void;

        auto getWeatherType() -> WeatherType;

        auto setWeatherType(WeatherType weatherType, bool isOverride) -> void;
    };

    inline Environment Environment;
}
