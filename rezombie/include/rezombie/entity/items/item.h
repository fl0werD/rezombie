#pragma once

#include "rezombie/core/object.h"
#include "cssdk/dll/player.h"

namespace rz
{
    using namespace amxx;

    class Item : public Object {
        int priceId_ = 0;
        int classIndex_ = 0;
        int giveForward_{};

      public:
        Item(
            std::string handle,
            int giveForward
        ) : Object(std::move(handle)),
            giveForward_(giveForward) {}

        auto getPrice() const -> int { return priceId_; }

        auto setPrice(int priceId) -> void { priceId_ = priceId; }

        auto getClass() const -> int { return classIndex_; }

        auto setClass(int classIndex) -> void { classIndex_ = classIndex; }

        auto getGiveForward() const { return giveForward_; }

        auto executeGive(int playerId, int itemId) const -> bool {
            // no need because consturctor
            if (getGiveForward() == FORWARD_INVALID) {
                return false;
            }
            if (ItemApi.GivePre(playerId, itemId) > ForwardReturn::Continue) {
                return false;
            }
            ExecuteForward(getGiveForward(), playerId);
            ItemApi.GivePost(playerId, itemId);
            return true;
        }
    };
}
