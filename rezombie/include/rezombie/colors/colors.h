#pragma once

#include "rezombie/entity/colors/color.h"
#include <string>
#include <optional>
#include <unordered_map>

namespace rz
{
    class Colors {
        std::unordered_map<std::string, Color> defaultColors_;
        std::unordered_map<std::string, Color> customColors_;

      public:
        Colors();

        auto add(const std::string& name, const Color& color) -> void {
            customColors_[name] = color;
        }

        auto parse(const std::string& color) -> std::optional<Color>;
    };

    inline Colors Colors;
}
