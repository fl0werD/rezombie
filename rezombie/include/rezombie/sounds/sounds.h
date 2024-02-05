#pragma once

#include <string>

namespace rz
{
    class Sounds {
        std::string lastAmbienceSound_;

        public:
            auto playAmbienceSound(const std::string& soundPath) -> void;
            auto stopAmbienceSound() -> void;
    };

    inline Sounds Sounds;
}
