#pragma once

#include <messages/user_message.h>
#include <cssdk/common/const.h>
#include <string>

namespace rz
{
    using namespace cssdk;
    using namespace message;

    class Weather
    {
      private:
        std::string skyName_;
        std::string lightInfo_ = "m";
        WeatherType weatherType_ = WeatherType::None;
        Color24 fogColor_ = {0, 0, 0};
        float fogDensity_ = 0.0001f;

      public:
        auto getSkyName() -> const std::string&;
        auto setSkyName(const std::string& skyName) -> void;
        auto getLight() -> const std::string&;
        auto setLight(const std::string& lightInfo) -> void;
        auto getWeatherType() -> WeatherType;
        auto setWeatherType(WeatherType weatherType) -> void;
        auto getFogColor() -> const Color24&;
        auto setFogColor(const Color24& color) -> void;
        auto getFogDensity() -> float;
        auto setFogDensity(float density) -> void;
    };

    inline Weather g_weather;
}
