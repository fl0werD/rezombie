#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/weapons/melee.h"
#include "rezombie/player/players.h"
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace vhooks;

    VirtualHook MeleeVirtuals::deploy(
        WEAPON_MELEE,
        HookIndex::Item_Deploy,
        &MeleeVirtuals::MeleeDeploy
    );

    auto MeleeVirtuals::MeleeDeploy() -> qboolean {
        const auto meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            return deploy.Call<qboolean>(this);
        }
        auto& melee = meleeRef->get();
        //EMIT_SOUND(m_pPlayer->edict(), CHAN_ITEM, "weapons/knife_deploy1.wav", 0.3, 2.4);
        WeaponDefaultDeploy(Players[player], this, 3, "knife");
        return melee.executeDeploy(EdictIndex(), player->EdictIndex());
    }

    VirtualHook MeleeVirtuals::primaryAttack(
        WEAPON_MELEE,
        HookIndex::Weapon_PrimaryAttack,
        &MeleeVirtuals::MeleePrimaryAttack
    );

    auto MeleeVirtuals::MeleePrimaryAttack() -> void {
        const auto meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            primaryAttack.Call(this);
            return;
        }
        auto& melee = meleeRef->get();
        primaryAttack.Call(this);
        // melee.executePrimaryAttack(EdictIndex(), player->EdictIndex(), clip, player->ammo[primary_ammo_type]);
    }

    VirtualHook MeleeVirtuals::secondaryAttack(
        WEAPON_MELEE,
        HookIndex::Weapon_SecondaryAttack,
        &MeleeVirtuals::MeleeSecondaryAttack
    );

    auto MeleeVirtuals::MeleeSecondaryAttack() -> void {
        const auto meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            secondaryAttack.Call(this);
            return;
        }
        auto& melee = meleeRef->get();
        secondaryAttack.Call(this);
        // melee.executeSecondaryAttack(EdictIndex(), player->EdictIndex(), weapon_state, player->fov);
    }
}
