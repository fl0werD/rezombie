#pragma once

#include <utility>

#include "rezombie/core/base_object.h"
#include "rezombie/modules/player_nightvision.h"

namespace rz::player
{
    class PlayerNightVision : public BaseObject {
      private:
        Color24 color_ = {255, 255, 255};

      public:
        PlayerNightVision(std::string handle) : BaseObject(std::move(handle)) {}

        auto getColor() const -> const Color24& { return color_; }

        auto setColor(Color24 color) -> void { color_ = std::move(color); }
    };
}
