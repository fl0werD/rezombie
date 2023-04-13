#pragma once

#include "rezombie/core/base_object.h"
#include "cssdk/dll/player.h"

namespace rz
{
    class Item : public BaseObject {
      private:
        int classIndex_ = 0;
        int price_ = 0;
        int giveForward_ = -1;

      public:
        Item(std::string handle, int classIndex, int giveForward) :
            BaseObject(std::move(handle)),
            classIndex_(classIndex),
            giveForward_(giveForward) {}

        auto getClass() const -> int { return classIndex_; }

        auto setClass(int classIndex) -> void { classIndex_ = classIndex; }

        auto getPrice() const -> int { return price_; }

        auto setPrice(int price) -> void { price_ = price; }

        auto getGiveForward() const { return giveForward_; }

        auto setGiveForward(int giveForward) { giveForward_ = giveForward; }

        auto executeGive(int player) const -> bool {
            if (getGiveForward() == -1) {
                return false;
            }
            ExecuteForward(getGiveForward(), player);
            return true;
        }
    };
}
