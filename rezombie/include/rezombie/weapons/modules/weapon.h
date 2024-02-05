#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/weapons/weapon.h"
#include "rezombie/entity/weapons/melee.h"

namespace rz
{
    using namespace cssdk;
    using namespace core;

    class WeaponModule : public Module<BaseWeapon> {
      public:
        WeaponModule() : Module("weapons") {}

        auto add(std::string handle, WeaponType weaponType) -> int {
            if (weaponType == WeaponType::Melee) {
                return Module::add(new Melee(std::move(handle)));
            } else {
                return Module::add(new Weapon(std::move(handle), weaponType));
            }
        }

        auto precache() -> void override {
            forEach([&](const auto& item) {
                precacheHud(item->getHudPath());
            });
        }

      private:
        static auto precacheHud(const std::string& hudPath) -> void {
            if (hudPath.empty()) {
                return;
            }
            PrecacheGeneric(hudPath.c_str());
        }
    };

    inline WeaponModule Weapons;
}
