#include "rezombie/map/environment.h"
#include "rezombie/main/util.h"
#include "rezombie/messages/engine_message.h"
#include "rezombie/messages/user_message.h"

namespace rz
{
    auto Environment::precache() -> void {
        const auto& skyName = getSkyName();
        if (skyName.empty()) {
            return;
        }
        precacheSkyByName(skyName);
        CvarSetString("sv_skyname", skyName.c_str());
        CvarSetFloat("sv_skycolor_r", 0);
        CvarSetFloat("sv_skycolor_g", 0);
        CvarSetFloat("sv_skycolor_b", 0);
    }

    auto Environment::reset() -> void {
        if (source_.getLight() != override_.getLight()) {
            override_.setLight(source_.getLight());
            netLightStyle(TO_ALL, 0, source_.getLight());
        }
        if (source_.getFog() != override_.getFog()) {
            override_.setFog(source_.getFog());
            sendFog(TO_ALL, source_.getFog());
        }
        if (source_.getWeatherType() != override_.getWeatherType()) {
            override_.setWeatherType(source_.getWeatherType());
            sendReceiveW(TO_ALL, source_.getWeatherType());
        }
    }

    auto Environment::getLight() -> const std::string& {
        return getEnvironment(true).getLight();
    }

    auto Environment::setLight(const std::string& light, bool isOverride) -> void {
        auto& env = getEnvironment(isOverride);
        if (env.getLight() == light) {
            return;
        }
        if (!isOverride) {
            source_.setLight(light);
        }
        override_.setLight(light);
        netLightStyle(TO_ALL, 0, light);
    }

    auto Environment::getFog() -> int {
        return getEnvironment(true).getFog();
    }

    auto Environment::setFog(int fogId, bool isOverride) -> void {
        auto& env = getEnvironment(isOverride);
        if (env.getFog() == fogId) {
            return;
        }
        if (!isOverride) {
            source_.setFog(fogId);
        }
        override_.setFog(fogId);
        sendFog(TO_ALL, fogId);
    }
/*
    auto Environment::getFogDensityPercentage() -> int {
        return getEnvironment(true).getFogDensityPercentage();
    }

    auto Environment::setFogDensityPercentage(int densityPercentage, bool isOverride) -> void {
        auto& env = getEnvironment(isOverride);
        if (env.getFogDensityPercentage() == densityPercentage) {
            return;
        }
        if (!isOverride) {
            source_.setFogDensityPercentage(densityPercentage);
        }
        override_.setFogDensityPercentage(densityPercentage);
        sendFog(TO_ALL, getFogColor(), densityPercentage);
    }
*/
    auto Environment::getWeatherType() -> WeatherType {
        return getEnvironment(true).getWeatherType();
    }

    auto Environment::setWeatherType(WeatherType weatherType, bool isOverride) -> void {
        auto& env = getEnvironment(isOverride);
        if (env.getWeatherType() == weatherType) {
            return;
        }
        if (!isOverride) {
            source_.setWeatherType(weatherType);
        }
        override_.setWeatherType(weatherType);
        sendReceiveW(TO_ALL, weatherType);
    }
}
