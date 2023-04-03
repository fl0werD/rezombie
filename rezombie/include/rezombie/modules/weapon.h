#pragma once

#include "rezombie/core/module.h"
#include "rezombie/weapons/weapon.h"
#include "rezombie/weapons/melee.h"
#include <core/strings/format.h>
#include <cssdk/dll/weapon_type.h>

namespace rz
{
    using namespace cssdk;
    using namespace core;
    using namespace weapon;

    class WeaponModule : public Module<BaseWeapon> {
      public:
        WeaponModule() : Module<BaseWeapon>("weapon") {}

        auto add(std::string handle, WeaponType weaponType) -> int {
            if (weaponType == WeaponType::Melee) {
                return Module::add(new Melee(std::move(handle)));
            } else {
                return Module::add(new Weapon(std::move(handle), weaponType));
            }
        }

        auto precache() -> void override {
            forEach([&](const auto& item) {
                precacheModel(item->getViewModel());
                precacheModel(item->getPlayerModel());
                precacheModel(item->getWorldModel());
                precacheWeaponHud(item->getHud());
            });
        }

        static auto precacheWeaponHud(const std::string& hudName) -> int {
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
