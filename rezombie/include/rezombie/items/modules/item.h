#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/items/item.h"

namespace rz
{
    class ItemModule : public Module<Item> {
      public:
        ItemModule() : Module("item") {}

        auto add(std::string handle, int giveForward) -> int {
            return Module::add(new Item(std::move(handle), giveForward));
        }
    };

    inline ItemModule Items;
}
