#include "rezombie/modules/weapon.h"
#include "rezombie/weapons/melee.h"
#include <metamod/utils.h>
#include <vhooks/vhooks.h>

namespace rz::weapon {
    using namespace cssdk;
    using namespace vhooks;

    constexpr auto melee = WEAPON_MELEE;
    VirtualHook MeleeVirtuals::deploy(melee, HookIndex::Item_Deploy, &MeleeVirtuals::MeleeDeploy);
    VirtualHook MeleeVirtuals::primaryAttack(melee, HookIndex::Weapon_PrimaryAttack, &MeleeVirtuals::MeleePrimaryAttack);
    VirtualHook MeleeVirtuals::secondaryAttack(melee, HookIndex::Weapon_SecondaryAttack, &MeleeVirtuals::MeleeSecondaryAttack);

    auto MeleeVirtuals::MeleeDeploy() -> qboolean {
        const auto meleeRef = weaponModule[vars->impulse];
        if (!meleeRef) {
            return deploy.Call<qboolean>(this);
        }
        auto& melee = meleeRef->get();
        return melee.executeDeploy(EdictIndex(), player->EdictIndex());
    }

    auto MeleeVirtuals::MeleePrimaryAttack() -> void {
        const auto meleeRef = weaponModule[vars->impulse];
        if (!meleeRef) {
            primaryAttack.Call(this);
            return;
        }
        auto& melee = meleeRef->get();
        // melee.executePrimaryAttack(EdictIndex(), player->EdictIndex(), clip, player->ammo[primary_ammo_type]);
    }

    auto MeleeVirtuals::MeleeSecondaryAttack() -> void {
        const auto meleeRef = weaponModule[vars->impulse];
        if (!meleeRef) {
            secondaryAttack.Call(this);
            return;
        }
        auto& melee = meleeRef->get();
        // melee.executeSecondaryAttack(EdictIndex(), player->EdictIndex(), weapon_state, player->fov);
    }
}
