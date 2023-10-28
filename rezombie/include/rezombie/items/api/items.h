#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    enum class ItemForward : int {
        Give_Pre,
        Give_Post,
        MAX_FORWARDS,
    };

    class AmxxItem : public AmxxFeature<ItemForward, ItemForward::MAX_FORWARDS> {
      public:
        AmxxItem() : AmxxFeature("item") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto GivePre(int playerId, int itemId) -> ForwardReturn;
        auto GivePost(int playerId, int itemId) -> void;
    };

    inline AmxxItem ItemApi;
}
