#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/weapons/melee.h"
#include "rezombie/weapons/api/melee.h"
#include "rezombie/player/players.h"
#include "rezombie/messages/user_message.h"
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace vhooks;

    VirtualHook MeleeVirtuals::addToPlayer(
        WEAPON_MELEE,
        HookIndex::Item_AddToPlayer,
        &MeleeVirtuals::MeleeAddToPlayer
    );

    auto MeleeVirtuals::MeleeAddToPlayer(PlayerBase* base) -> qboolean {
        const auto meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            return addToPlayer.Call<qboolean>(this, base);
        }
        const auto& melee = meleeRef->get();
        player = base;
        auto& player = Players[base];
        const auto& itemInfo = GetCsPlayerItem()->item_info;
        player.setWeapons(player.getWeapons() | (1 << toInt(itemInfo.id)));
        sendWeaponList(
            player,
            melee.getHud().c_str(),
            primary_ammo_type,
            itemInfo.max_ammo1,
            secondary_ammo_type,
            itemInfo.max_ammo2,
            itemInfo.slot,
            itemInfo.position,
            itemInfo.id,
            itemInfo.flags
        );
        sendWeapPickup(player, itemInfo.id);
        return true;
    }

    VirtualHook MeleeVirtuals::deploy(
        WEAPON_MELEE,
        HookIndex::Item_Deploy,
        &MeleeVirtuals::MeleeDeploy
    );

    auto MeleeVirtuals::MeleeDeploy() -> qboolean {
        const auto& meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            return deploy.Call<qboolean>(this);
        }
        bool result;
        auto& owner = Players[player];
        auto& melee = dynamic_cast<Melee&>(meleeRef->get());
        if (melee.getDeployForward() != FORWARD_INVALID) {
            result = melee.executeDeploy(EdictIndex(), owner);
        } else {
            result = WeaponDefaultDeploy(owner, this, KNIFE_DRAW, "knife");
            if (result) {
                UTIL_EmitSound(GetEdict(), SoundChannel::Item, melee.getRandom(MeleeSound::Deploy));
            }
        }
        return result;
    }

    VirtualHook MeleeVirtuals::maxSpeed(
        WEAPON_MELEE,
        HookIndex::Item_GetMaxSpeed,
        &MeleeVirtuals::MeleeMaxSpeed
    );

    auto MeleeVirtuals::MeleeMaxSpeed() -> float {
        const auto& meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            return maxSpeed.Call<float>(this);
        }
        auto result = 250;
        const auto& melee = meleeRef->get();
        if (melee.getMaxSpeedForward() == FORWARD_INVALID) {
            const auto& owner = Players[player];
            result = melee.executeMaxSpeed(EdictIndex(), owner, player->fov);
        }
        return static_cast<float>(result);
    }

    VirtualHook MeleeVirtuals::primaryAttack(
        WEAPON_MELEE,
        HookIndex::Weapon_PrimaryAttack,
        &MeleeVirtuals::MeleePrimaryAttack
    );

    auto MeleeVirtuals::MeleePrimaryAttack() -> void {
        const auto& meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            primaryAttack.Call(this);
            return;
        }
        auto& owner = Players[player];
        auto result = MeleeAttackResult::Miss;
        if (MeleeApi.AttackPre(owner, Primary) <= ForwardReturn::Continue) {
            auto& melee = dynamic_cast<Melee&>(meleeRef->get());
            if (melee.getPrimaryAttackForward() != FORWARD_INVALID) {
                result = melee.executeMeleePrimaryAttack(EdictIndex(), owner);
            } else {
                result = MeleeDefaultAttack(owner, dynamic_cast<Knife*>(this), 15, 32);
                owner.SendWeaponAnim(RandomLong(KNIFE_MIDDLE_ATTACK1_HIT, KNIFE_MIDDLE_ATTACK2_HIT));
                switch (result) {
                    case MeleeAttackResult::Miss: {
                        next_primary_attack = 0.35f;
                        next_secondary_attack = 0.5f;
                        UTIL_EmitSound(GetEdict(), SoundChannel::Weapon, melee.getRandom(MeleeSound::Slash));
                        break;
                    }
                    case MeleeAttackResult::Hit: {
                        next_primary_attack = 0.4f;
                        next_secondary_attack = 0.5f;
                        UTIL_EmitSound(GetEdict(), SoundChannel::Weapon, melee.getRandom(MeleeSound::Hit));
                        break;
                    }
                    case MeleeAttackResult::HitWall: {
                        next_primary_attack = 0.4f;
                        next_secondary_attack = 0.5f;
                        UTIL_EmitSound(GetEdict(), SoundChannel::Item, melee.getRandom(MeleeSound::HitWall));
                        break;
                    }
                }
                time_weapon_idle = 2.f;
            }
        }
        MeleeApi.AttackPost(owner, Primary, result);
    }

    VirtualHook MeleeVirtuals::secondaryAttack(
        WEAPON_MELEE,
        HookIndex::Weapon_SecondaryAttack,
        &MeleeVirtuals::MeleeSecondaryAttack
    );

    auto MeleeVirtuals::MeleeSecondaryAttack() -> void {
        const auto& meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            secondaryAttack.Call(this);
            return;
        }
        auto& owner = Players[player];
        auto result = MeleeAttackResult::Miss;
        if (MeleeApi.AttackPre(owner, Secondary) <= ForwardReturn::Continue) {
            auto& melee = dynamic_cast<Melee&>(meleeRef->get());
            if (melee.getSecondaryAttackForward() != FORWARD_INVALID) {
                result = melee.executeMeleeSecondaryAttack(EdictIndex(), owner);
            } else {
                result = MeleeDefaultAttack(owner, dynamic_cast<Knife*>(this), 65, 48, 3);
                switch (result) {
                    case MeleeAttackResult::Miss: {
                        next_primary_attack = 1.f;
                        next_secondary_attack = 1.f;
                        owner.SendWeaponAnim(KNIFE_STAB_MISS);
                        UTIL_EmitSound(GetEdict(), SoundChannel::Weapon, melee.getRandom(MeleeSound::Slash));
                        break;
                    }
                    case MeleeAttackResult::Hit: {
                        next_primary_attack = 1.1f;
                        next_secondary_attack = 1.1f;
                        owner.SendWeaponAnim(KNIFE_STAB_HIT);
                        UTIL_EmitSound(GetEdict(), SoundChannel::Weapon, melee.getRandom(MeleeSound::Stab));
                        break;
                    }
                    case MeleeAttackResult::HitWall: {
                        next_primary_attack = 1.1f;
                        next_secondary_attack = 1.1f;
                        owner.SendWeaponAnim(KNIFE_STAB_HIT);
                        UTIL_EmitSound(GetEdict(), SoundChannel::Item, melee.getRandom(MeleeSound::HitWall));
                        break;
                    }
                }
                time_weapon_idle = 2.f;
            }
        }
        MeleeApi.AttackPost(owner, Secondary, result);
    }

    VirtualHook MeleeVirtuals::idle(
        WEAPON_MELEE,
        HookIndex::Weapon_WeaponIdle,
        &MeleeVirtuals::MeleeIdle
    );

    auto MeleeVirtuals::MeleeIdle() -> void {
        const auto& meleeRef = Weapons[vars->impulse];
        if (!meleeRef) {
            idle.Call(this);
            return;
        }
        auto& owner = Players[player];
        const auto& melee = meleeRef->get();
        if (melee.getIdleForward() != FORWARD_INVALID) {
            melee.executeIdle(EdictIndex(), owner, time_weapon_idle);
        } else {
            ResetEmptySound();
            if (time_weapon_idle > 0.f) {
                return;
            }
            time_weapon_idle = 20.f;
            owner.SendWeaponAnim(KNIFE_IDLE);
        }
    }

    VirtualHook MeleeVirtuals::create(
        WEAPON_MELEE,
        HookIndex::OnCreate,
        &MeleeVirtuals::MeleeCreate
    );

    auto MeleeVirtuals::MeleeCreate() -> void {
        create.Call(this);
        link = new Extras();
    }

    VirtualHook MeleeVirtuals::destroy(
        WEAPON_MELEE,
        HookIndex::OnDestroy,
        &MeleeVirtuals::MeleeDestroy
    );

    auto MeleeVirtuals::MeleeDestroy() -> void {
        delete link;
        destroy.Call(this);
    }

    /*
        void FindHullIntersection(const Vector& vecSrc, TraceResult& tr, float* mins, float* maxs, Edict* pEntity) {
            int i, j, k;
            float distance;
            float* minmaxs[2] = {mins, maxs};
            TraceResult tmpTrace;
            Vector vecHullEnd = tr.end_position;
            Vector vecEnd;

            distance = 1e6f;

            vecHullEnd = vecSrc + ((vecHullEnd - vecSrc) * 2);
            //g_global_vars->trace_flags = FTRACE_KNIFE;
            TraceLine(vecSrc, vecHullEnd, TR_IGNORE_NONE, pEntity, &tmpTrace);

            if (tmpTrace.fraction < 1.0f) {
                tr = tmpTrace;
                return;
            }

            for (i = 0; i < 2; i++) {
                for (j = 0; j < 2; j++) {
                    for (k = 0; k < 2; k++) {
                        vecEnd.x = vecHullEnd.x + minmaxs[i][0];
                        vecEnd.y = vecHullEnd.y + minmaxs[j][1];
                        vecEnd.z = vecHullEnd.z + minmaxs[k][2];

                        //gpGlobals->trace_flags = FTRACE_KNIFE;
                        TraceLine(vecSrc, vecEnd, TR_IGNORE_NONE, pEntity, &tmpTrace);

                        if (tmpTrace.fraction < 1.0f) {
                            auto thisDistance = (tmpTrace.end_position - vecSrc).Length();

                            if (thisDistance < distance) {
                                tr = tmpTrace;
                                distance = thisDistance;
                            }
                        }
                    }
                }
            }
        }

        void CKnife::WeaponIdle() {
            ResetEmptySound();
            if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
                return;

            m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 20.0f;
            SendWeaponAnim(KNIFE_IDLE, UseDecrement() != FALSE);
        }
    */
}
