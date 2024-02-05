#include "rezombie/weapons/weapons.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/players.h"
#include "rezombie/messages/user_message.h"
#include <metamod/utils.h>
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace core;
    using namespace vhooks;

    VirtualHook WeaponVirtuals::spawn(
        WEAPON_PLACEHOLDER,
        HookIndex::Spawn,
        &WeaponVirtuals::HolderSpawn
    );

    auto WeaponVirtuals::HolderSpawn() -> void {
        spawn.Call(this);
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return;
        }
        const auto& itemInfo = GetCsPlayerItem()->item_info;
        id = itemInfo.id;
        clip = itemInfo.max_clip;
        primary_ammo_type = toInt(itemInfo.id);
        secondary_ammo_type = -1;
    }

    VirtualHook WeaponVirtuals::addToPlayer(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_AddToPlayer,
        &WeaponVirtuals::HolderAddToPlayer
    );

    auto WeaponVirtuals::HolderAddToPlayer(PlayerBase* base) -> qboolean {
        const auto weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return addToPlayer.Call<qboolean>(this, base);
        }
        const auto& weapon = weaponRef->get();
        player = base;
        auto& player = Players[base];
        const auto& itemInfo = GetCsPlayerItem()->item_info;
        player.setWeapons(player.getWeapons() | (1 << toInt(itemInfo.id)));
        sendWeaponList(
            player,
            weapon.getHud().c_str(),
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

    VirtualHook WeaponVirtuals::getItemInfo(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_GetItemInfo,
        &WeaponVirtuals::HolderGetItemInfo
    );

    auto WeaponVirtuals::HolderGetItemInfo(ItemInfo* info) -> qboolean {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return getItemInfo.Call<qboolean>(this, info);
        }
        const auto& weapon = weaponRef->get();
        const auto fakeId = static_cast<WeaponId>(vars->i_user1);
        id = fakeId;
        info->slot = toInt(weapon.getSlot()) - 1;
        info->position = regamedll_api::GetItemInfo(fakeId)->position;
        info->ammo1 = nullptr;
        info->max_ammo1 = weapon.getMaxAmmo();
        info->ammo2 = nullptr;
        info->max_ammo2 = -1;
        info->name = weapon.getHandle().c_str();
        info->max_clip = weapon.getMaxClip();
        info->id = fakeId;
        info->flags = weapon.getFlags();
        info->weight = weapon.getWeight();
        return true;
    }

    VirtualHook WeaponVirtuals::deploy(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_Deploy,
        &WeaponVirtuals::HolderDeploy
    );

    auto WeaponVirtuals::HolderDeploy() -> qboolean {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return deploy.Call<qboolean>(this);
        }
        const auto& weapon = weaponRef->get();
        const auto& player = Players[this->player];
        const auto result = weapon.executeDeploy(EdictIndex(), player);
        if (result) {
            vars->effects &= ~EF_NO_DRAW;
        }
        return result;
    }

    VirtualHook WeaponVirtuals::holster(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_Holster,
        &WeaponVirtuals::HolderHolster
    );

    auto WeaponVirtuals::HolderHolster(int skipLocal) -> void {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            holster.Call(this, skipLocal);
            return;
        }
        const auto& weapon = weaponRef->get();
        auto& player = Players[this->player];
        in_reload = false;
        vars->effects |= EF_NO_DRAW;
        player.setViewModel(0);
        player.setWeaponModel(0);
        weapon.executeHolster(EdictIndex(), player);
    }

    VirtualHook WeaponVirtuals::attachToPlayer(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_AttachToPlayer,
        &WeaponVirtuals::HolderAttachToPlayer
    );

    auto WeaponVirtuals::HolderAttachToPlayer(PlayerBase* base) -> void {
        const auto& player = Players[base];
        vars->move_type = MoveTypeEntity::Follow;
        vars->solid = SolidType::NotSolid;
        vars->aim_entity = player;
        vars->owner = player;
        vars->next_think = 0;
        SetThink(nullptr);
        SetTouch(nullptr);
        if (static_cast<PlayerItemBase*>(player.getActiveItem()) != this) {
            vars->effects |= EF_NO_DRAW;
        }
    }

    VirtualHook WeaponVirtuals::updateClientData(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_UpdateClientData,
        &WeaponVirtuals::HolderUpdateClientData
    );

    // TODO: why?
    auto WeaponVirtuals::HolderUpdateClientData(PlayerBase* base) -> qboolean {
        const auto weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return updateClientData.Call<qboolean>(this, base);
        }
        const auto& player = Players[base];
        auto send = false;
        auto state = 0;
        const PlayerItemBase* activeItem = player.getActiveItem();
        if (activeItem == this) {
            state = base->on_target ? WEAPON_IS_ON_TARGET : 1;
        }
        if (!base->weapon) {
            send = true;
        }
        if (activeItem == this || base->client_active_item == this) {
            if (activeItem != base->client_active_item) {
                send = true;
            }
        }
        if (clip != client_clip || state != client_weapon_state ||
            static_cast<int>(base->field_of_view) != base->client_fov) {
            send = true;
        }
        if (send) {
            sendCurWeapon(player, state, id, clip);
            client_clip = clip;
            client_weapon_state = state;
            base->weapon = true;
        }
        if (next) {
            next->UpdateClientData(base);
        }
        return true;
    }

    VirtualHook WeaponVirtuals::maxSpeed(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_GetMaxSpeed,
        &WeaponVirtuals::HolderMaxSpeed
    );

    auto WeaponVirtuals::HolderMaxSpeed() -> float {
        const auto weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return maxSpeed.Call<float>(this);
        }
        const auto& weapon = weaponRef->get();
        return static_cast<float>(weapon.executeMaxSpeed(EdictIndex(), player->EdictIndex(), player->fov));
    }

    VirtualHook WeaponVirtuals::itemSlot(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_ItemSlot,
        &WeaponVirtuals::HolderItemSlot
    );

    auto WeaponVirtuals::HolderItemSlot() -> InventorySlot {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            return itemSlot.Call<InventorySlot>(this);
        }
        const auto& weapon = weaponRef->get();
        return weapon.getSlot();
    }

    VirtualHook WeaponVirtuals::primaryAttack(
        WEAPON_PLACEHOLDER,
        HookIndex::Weapon_PrimaryAttack,
        &WeaponVirtuals::HolderPrimaryAttack
    );

    auto WeaponVirtuals::HolderPrimaryAttack() -> void {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            primaryAttack.Call(this);
            return;
        }
        const auto& weapon = weaponRef->get();
        weapon.executePrimaryAttack(EdictIndex(), player->EdictIndex(), clip, player->ammo[primary_ammo_type]);
    }

    VirtualHook WeaponVirtuals::secondaryAttack(
        WEAPON_PLACEHOLDER,
        HookIndex::Weapon_SecondaryAttack,
        &WeaponVirtuals::HolderSecondaryAttack
    );

    auto WeaponVirtuals::HolderSecondaryAttack() -> void {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            secondaryAttack.Call(this);
            return;
        }
        const auto& weapon = weaponRef->get();
        weapon.executeSecondaryAttack(EdictIndex(), player->EdictIndex(), weapon_state, player->fov);
    }

    VirtualHook WeaponVirtuals::reload(
        WEAPON_PLACEHOLDER,
        HookIndex::Weapon_Reload,
        &WeaponVirtuals::HolderReload
    );

    auto WeaponVirtuals::HolderReload() -> void {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            reload.Call(this);
            return;
        }
        const auto& weapon = weaponRef->get();
        weapon.executeReload(EdictIndex(), player->EdictIndex());
    }

    VirtualHook WeaponVirtuals::idle(
        WEAPON_PLACEHOLDER,
        HookIndex::Weapon_WeaponIdle,
        &WeaponVirtuals::HolderIdle
    );

    auto WeaponVirtuals::HolderIdle() -> void {
        const auto& weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            idle.Call(this);
            return;
        }
        const auto& weapon = weaponRef->get();
        weapon.executeIdle(EdictIndex(), player->EdictIndex(), time_weapon_idle);
    }

    // void CBasePlayerWeapon::EjectBrassLate()
    //{
    //     UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

    //    Vector vecUp = RANDOM_FLOAT(100, 150) * gpGlobals->v_up;
    //    Vector vecRight = RANDOM_FLOAT(50, 70) * gpGlobals->v_right;

    //    Vector vecShellVelocity = (m_pPlayer->pev->velocity + vecRight + vecUp) + gpGlobals->v_forward * 25;
    //    int soundType = (m_iId == WEAPON_XM1014 || m_iId == WEAPON_M3) ? 2 : 1;

    //    EjectBrass(pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -9 + gpGlobals->v_forward * 16,
    //      gpGlobals->v_right * -9, vecShellVelocity, pev->angles.y, m_iShellId, soundType, m_pPlayer->entindex());

    //    const Vector &vecOrigin, const Vector &vecLeft, const Vector &vecVelocity, float rotation, int models,
    //      int soundtype, int entityIndex

    //    MessageBegin(MessageType::Pvs, gmsgBrass, vecOrigin);
    //    WriteByte(TE_MODEL);
    //    WriteCoord(vecOrigin.x);
    //    WriteCoord(vecOrigin.y);
    //    WriteCoord(vecOrigin.z);
    //    WriteCoord(vecLeft.x);
    //    WriteCoord(vecLeft.y);
    //    WriteCoord(vecLeft.z);
    //    WriteCoord(vecVelocity.x);
    //    WriteCoord(vecVelocity.y);
    //    WriteCoord(vecVelocity.z);
    //    WriteAngle(rotation);
    //    WriteShort(models);
    //    WriteByte(soundtype);
    //    WriteByte(25); // life
    //    WriteByte(entityIndex);
    //    MessageEnd();
    //}

    VirtualHook WeaponVirtuals::postFrame(
        WEAPON_PLACEHOLDER,
        HookIndex::Item_PostFrame,
        &WeaponVirtuals::HolderPostFrame
    );

    auto WeaponVirtuals::HolderPostFrame() -> void {
        const auto weaponRef = Weapons[vars->impulse];
        if (!weaponRef) {
            postFrame.Call(this);
            return;
        }
        auto& baseWeapon = weaponRef->get();
        auto usableButtons = player->vars->button;
        const auto itemInfo = GetCsPlayerItem()->item_info;
        if (baseWeapon.getPrimaryAttackForward() == -1) {
            usableButtons &= ~IN_ATTACK;
        }
        if (baseWeapon.getSecondaryAttackForward() == -1) {
            usableButtons &= ~IN_ATTACK2;
        }
        const auto weapon = dynamic_cast<Weapon*>(&baseWeapon);
        if (weapon != nullptr && famas_shoot != 0.f && g_global_vars->time > famas_shoot) {
            weapon->executeFireRemaining(EdictIndex(), player->EdictIndex(), clip, famas_shots_fired);
        }
        if (next_primary_attack <= 0.0) {
            if (player->resume_zoom) {
                player->fov = player->last_zoom;
                player->vars->field_of_view = static_cast<float>(player->fov);
                if (player->fov == player->last_zoom) {
                    player->resume_zoom = false;
                }
            }
        }
        if (player->eject_brass != 0 && player->eject_brass <= g_global_vars->time) {
            player->eject_brass = 0;
            // EjectBrassLate();
        }
        if (!(usableButtons & IN_ATTACK)) {
            last_fire_time = 0;
        }
        if (in_reload && player->next_attack <= 0.0) {
            const auto difference = std::min(itemInfo.max_clip - clip, player->ammo[primary_ammo_type]);
            clip += difference;
            // if (player->infinite_ammo || weapon.isInfiniteAmmo())
            // if (refill_bpammo_weapons.value < 3.0f)
            {
                player->ammo[primary_ammo_type] -= difference;
            }
            in_reload = false;
            // executeReloadEnd?
        }
        if ((usableButtons & IN_ATTACK2) && next_secondary_attack <= 0.f) {
            if (!player->ammo[secondary_ammo_type]) {
                fire_on_empty = true;
            }
            SecondaryAttack();
            player->vars->button &= ~IN_ATTACK2;
        } else if ((player->vars->button & IN_ATTACK) && next_primary_attack <= 0.f) {
            if ((!clip && itemInfo.ammo1) ||
                (itemInfo.max_clip == WEAPON_NO_CLIP && !player->ammo[primary_ammo_type])) {
                fire_on_empty = true;
            }
            if (player->can_shoot && GameRules.isCanMove()) {
                if (player->vars->water_level == 3 && (itemInfo.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER)) {
                    PlayEmptySound();
                    next_primary_attack = 0.15f;
                } else {
                    PrimaryAttack();
                }
            }
        } else if ((usableButtons & IN_RELOAD) && itemInfo.max_clip != WEAPON_NO_CLIP && !in_reload &&
                   next_primary_attack < 0.f) {
            if (famas_shoot == 0) {
                Reload();
            }
        } else if (!(usableButtons & (IN_ATTACK | IN_ATTACK2))) {
            if (delay_fire) {
                delay_fire = false;
                if (shots_fired > 15) {
                    shots_fired = 15;
                }
                decrease_shots_fired = g_global_vars->time + 0.4f;
            }
            fire_on_empty = false;
            if (baseWeapon.getWeaponType() == WeaponType::Secondary) {
                shots_fired = 0;
            } else if (shots_fired > 0 && decrease_shots_fired < g_global_vars->time) {
                decrease_shots_fired = g_global_vars->time + 0.0225f;
                shots_fired--;
                if (weapon != nullptr && shots_fired == 0) {
                    accuracy = weapon->getBaseAccuracy();
                }
            }
            if (IsUseable() || next_primary_attack < 0.f) {
                if (!clip && !(itemInfo.flags & ITEM_FLAG_NO_AUTO_RELOAD)) {
                    if (famas_shoot == 0) {
                        Reload();
                        return;
                    }
                }
            }
            // HandleInfiniteAmmo();
            WeaponIdle();
            return;
        }
        // HandleInfiniteAmmo();
    }

    VirtualHook WeaponVirtuals::create(
        WEAPON_PLACEHOLDER,
        HookIndex::OnCreate,
        &WeaponVirtuals::HolderCreate
    );

    auto WeaponVirtuals::HolderCreate() -> void {
        create.Call(this);
        link = new Extras();
    }

    VirtualHook WeaponVirtuals::destroy(
        WEAPON_PLACEHOLDER,
        HookIndex::OnDestroy,
        &WeaponVirtuals::HolderDestroy
    );

    auto WeaponVirtuals::HolderDestroy() -> void {
        delete link;
        destroy.Call(this);
    }
}
