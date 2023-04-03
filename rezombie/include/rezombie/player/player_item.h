#pragma once

#include "rezombie/core/base_object.h"
#include <cssdk/dll/player.h>

namespace rz::player
{
    class PlayerItem : public BaseObject {
      private:
        int classIndex_ = 0;
        int price_ = 0;

      public:
        PlayerItem(std::string handle, int classIndex) :
            BaseObject(std::move(handle)),
            classIndex_(classIndex) {}

        auto getClass() const -> int { return classIndex_; }

        auto getPrice() const -> int { return price_; }

        auto setPrice(int price) -> void { price_ = price; }
    };
}
