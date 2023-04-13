#pragma once

#include "rezombie/core/amxx_feature.h"
#include "rezombie/gamerules/game_rules.h"
#include <array>

namespace rz
{
    enum class ItemVars : int {
        Handle,
        Name,
        Class,
        Price,
    };

    enum class ItemForward : int {
        Given,
        MAX_FORWARDS,
    };

    class AmxxItem : public AmxxFeature<ItemForward, ItemForward::MAX_FORWARDS> {
      public:
        AmxxItem() : AmxxFeature("item") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto Given(int playerIndex, int itemIndex) -> void;
    };

    inline AmxxItem amxxItem;
}
