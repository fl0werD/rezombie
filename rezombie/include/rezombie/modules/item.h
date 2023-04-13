#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/item.h"

namespace rz
{
    class ItemModule : public Module<Item> {
      public:
        ItemModule() : Module<Item>("item") {}

        auto add(std::string handle, int classIndex, int giveForward) -> int {
            return Module::add(new Item(std::move(handle), classIndex, giveForward));
        }
    };

    inline ItemModule itemModule;
}
