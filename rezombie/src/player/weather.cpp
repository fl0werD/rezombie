#include "rezombie/entity/weather.h"
#include "rezombie/util.h"
#include <algorithm>
#include <string>

namespace rz
{
    auto Weather::getSkyName() -> const std::string&
    {
        return skyName_;
    }

    auto Weather::setSkyName(const std::string& skyName) -> void
    {
        skyName_ = std::move(skyName);
    }

    auto Weather::getLight() -> const std::string&
    {
        return lightInfo_;
    }

    auto Weather::setLight(const std::string& lightInfo) -> void
    {
        lightInfo_ = std::move(lightInfo);
    }

    auto Weather::getWeatherType() -> WeatherType
    {
        return weatherType_;
    }

    auto Weather::setWeatherType(WeatherType weatherType) -> void
    {
        weatherType_ = weatherType;
    }

    auto Weather::getFogColor() -> const Color24&
    {
        return fogColor_;
    }

    auto Weather::setFogColor(const Color24& color) -> void
    {
        fogColor_ = std::move(color);
    }

    auto Weather::getFogDensity() -> float
    {
        return fogDensity_;
    }

    auto Weather::setFogDensity(float density) -> void
    {
        fogDensity_ = std::clamp(density, 0.0001f, 0.25f);
    }
}
