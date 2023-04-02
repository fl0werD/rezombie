#pragma once

#include "rezombie/core/module.h"
#include "rezombie/weapons/weapon.h"
#include <core/strings/format.h>
#include <cssdk/dll/weapon_type.h>

namespace rz
{
    using namespace cssdk;
    using namespace core;
    using namespace weapon;

    class WeaponModule : public Module<Weapon>
    {
      public:
        WeaponModule() : Module<Weapon>("weapon")
        {
        }

        auto add(std::string handle, WeaponType weaponType) -> int
        {
            return Module::add(Weapon(std::move(handle), weaponType));
        }

        auto precache() -> void override
        {
            forEach(
              [&](const auto& item)
              {
                  precacheModel(item.getViewModel());
                  precacheModel(item.getPlayerModel());
                  precacheModel(item.getWorldModel());
                  precacheWeaponHud(item.getHud());
              }
            );
        }

        static auto precacheWeaponHud(const std::string& hudName) -> int
        {
            if (hudName.empty()) {
                return -1;
            }
            const auto spritePath = str::Format("sprites/%s.txt", hudName.c_str());
            // return metamod::engine::PrecacheGeneric(spritePath.c_str());
            return 0;
        }
    };

    inline WeaponModule weaponModule;
}
