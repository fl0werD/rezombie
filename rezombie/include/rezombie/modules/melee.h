#pragma once

#include "rezombie/core/module.h"
#include "rezombie/weapons/melee.h"

namespace rz
{
    using namespace rz::weapon;

    class MeleeModule : public Module<Melee>
    {
      public:
        MeleeModule() : Module<Melee>("melee")
        {
        }

        auto add(std::string handle) -> int
        {
            return Module::add(Melee(std::move(handle)));
        }

        auto precache() -> void
        {
            forEach(
              [this](const auto& item)
              {
                  precacheModel(item.getViewModel());
                  precacheModel(item.getPlayerModel());
                  precacheModel(item.getWorldModel());
              }
            );
        }
    };

    inline MeleeModule meleeModule;
}
