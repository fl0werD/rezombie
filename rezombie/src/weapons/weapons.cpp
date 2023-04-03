#include "rezombie/weapons/weapons.h"
#include "rezombie/gamerules/game_rules.h"
#include <messages/user_message.h>
#include "rezombie/modules/weapon.h"
#include "rezombie/player/players.h"
#include <metamod/utils.h>
#include <mhooks/metamod.h>
#include <mhooks/reapi.h>
#include <vhooks/vhooks.h>
#include <array>

namespace rz::weapon
{
    using namespace cssdk;
    using namespace vhooks;
    using namespace rz::player;
    using namespace message;

    constexpr auto holder = WEAPON_PLACEHOLDER;
    VirtualHook WeaponVirtuals::spawn(holder, HookIndex::Spawn, &WeaponVirtuals::HolderSpawn);
    VirtualHook WeaponVirtuals::addToPlayer(holder, HookIndex::Item_AddToPlayer, &WeaponVirtuals::HolderAddToPlayer);
    VirtualHook WeaponVirtuals::getItemInfo(holder, HookIndex::Item_GetItemInfo, &WeaponVirtuals::HolderGetItemInfo);
    VirtualHook WeaponVirtuals::deploy(holder, HookIndex::Item_Deploy, &WeaponVirtuals::HolderDeploy);
    VirtualHook WeaponVirtuals::holster(holder, HookIndex::Item_Holster, &WeaponVirtuals::HolderHolster);
    VirtualHook WeaponVirtuals::updateClientData(
      holder, HookIndex::Item_UpdateClientData, &WeaponVirtuals::HolderUpdateClientData
    );
    VirtualHook WeaponVirtuals::maxSpeed(holder, HookIndex::Item_GetMaxSpeed, &WeaponVirtuals::HolderMaxSpeed);
    VirtualHook WeaponVirtuals::itemSlot(holder, HookIndex::Item_ItemSlot, &WeaponVirtuals::HolderItemSlot);
    VirtualHook
      WeaponVirtuals::primaryAttack(holder, HookIndex::Weapon_PrimaryAttack, &WeaponVirtuals::HolderPrimaryAttack);
    VirtualHook WeaponVirtuals::secondaryAttack(
      holder, HookIndex::Weapon_SecondaryAttack, &WeaponVirtuals::HolderSecondaryAttack
    );
    VirtualHook WeaponVirtuals::reload(holder, HookIndex::Weapon_Reload, &WeaponVirtuals::HolderReload);
    VirtualHook WeaponVirtuals::idle(holder, HookIndex::Weapon_WeaponIdle, &WeaponVirtuals::HolderIdle);
    VirtualHook WeaponVirtuals::postFrame(holder, HookIndex::Item_PostFrame, &WeaponVirtuals::HolderPostFrame);

    auto GetWeaponData_Post(const GameDllGetWeaponDataMChain& chain, Edict* client, WeaponData* data) -> qboolean
    {
        chain.CallNext(client, data);
        const auto& player = player::players[client];
        const auto activeItem = player.getActiveItem();
        if (activeItem != nullptr) {
            const auto weaponRef = weaponModule[activeItem->vars->impulse];
            if (weaponRef) {
                const auto itemInfo = activeItem->GetCsPlayerItem()->item_info;
                // data->id = toInt(itemInfo.id);
                data->clip = itemInfo.max_clip;
            }
        }
        return true;
    }

    auto WeaponBoxSetModel(const ReGameWeaponBoxSetModelMChain& chain, WeaponBox* weaponBox, const char* modelName)
      -> void
    {
        for (auto item : weaponBox->player_items) {
            while (item != nullptr) {
                const auto weaponRef = weaponModule[item->vars->impulse];
                if (!weaponRef) {
                    chain.CallNext(weaponBox, modelName);
                    return;
                }
                const auto& weapon = weaponRef->get();
                if (!weapon.getWorldModel().empty()) {
                    chain.CallNext(weaponBox, weapon.getWorldModel().c_str());
                    return;
                }
                item = item->next;
            }
        }
        chain.CallNext(weaponBox, modelName);
    }

    auto RegisterHooks() -> void
    {
        using namespace core;
        using namespace mhooks;

        MHookGameDllGetWeaponData(DELEGATE_ARG<GetWeaponData_Post>, true);
        MHookReGameWeaponBoxSetModel(DELEGATE_ARG<WeaponBoxSetModel>);
    }

    auto WeaponVirtuals::HolderSpawn() -> void
    {
        spawn.Call(this);
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return;
        }
        const auto itemInfo = GetCsPlayerItem()->item_info;
        id = itemInfo.id;
        clip = itemInfo.max_clip;
        primary_ammo_type = toInt(itemInfo.id);
        secondary_ammo_type = -1;
    }

    auto WeaponVirtuals::HolderAddToPlayer(PlayerBase* basePlayer) -> qboolean
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return addToPlayer.Call<qboolean>(this, basePlayer);
        }
        const auto& weapon = weaponRef->get();
        player = basePlayer;
        auto& player = players[basePlayer];
        auto itemInfo = GetCsPlayerItem()->item_info;
        player.setWeapons(player.getWeapons() | (1 << toInt(itemInfo.id)));
        sendWeaponList(
          player, weapon.getHud().c_str(), primary_ammo_type, itemInfo.max_ammo1, secondary_ammo_type,
          itemInfo.max_ammo2, itemInfo.slot, itemInfo.position, itemInfo.id, itemInfo.flags
        );
        sendWeapPickup(player, itemInfo.id);
        return true;
    }

    auto WeaponVirtuals::HolderGetItemInfo(ItemInfo* info) -> qboolean
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return getItemInfo.Call<qboolean>(this, info);
        }
        auto& weapon = weaponRef->get();
        const auto fakeId = static_cast<WeaponId>(vars->i_user1);
        id = fakeId;
        info->slot = toInt(weapon.getInventorySlot()) - 1;
        info->position = core::regamedll_api::GetItemInfo(fakeId)->position;
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

    auto WeaponVirtuals::HolderDeploy() -> qboolean
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return deploy.Call<qboolean>(this);
        }
        auto& weapon = weaponRef->get();
        return weapon.executeDeploy(EdictIndex(), player->EdictIndex());
    }

    auto WeaponVirtuals::HolderHolster(int skipLocal) -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            holster.Call(this, skipLocal);
            return;
        }
        auto& weapon = weaponRef->get();
        weapon.executeHolster(EdictIndex(), player->EdictIndex());
    }

    auto WeaponVirtuals::HolderUpdateClientData(PlayerBase* basePlayer) -> qboolean
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return updateClientData.Call<qboolean>(this, basePlayer);
        }
        const auto& player = players[basePlayer];
        bool send = false;
        int state = 0;
        const auto activeItem = player.getActiveItem();
        if (activeItem == this) {
            state = basePlayer->on_target ? WEAPON_IS_ON_TARGET : 1;
        }
        if (!basePlayer->weapon) {
            send = true;
        }
        if (activeItem == this || basePlayer->client_active_item == this) {
            if (activeItem != basePlayer->client_active_item) {
                send = true;
            }
        }
        if (clip != client_clip || state != client_weapon_state || basePlayer->field_of_view != basePlayer->client_fov) {
            send = true;
        }
        if (send) {
            sendCurWeapon(player, state, id, clip);
            client_clip = clip;
            client_weapon_state = state;
            basePlayer->weapon = true;
        }
        if (next) {
            next->UpdateClientData(basePlayer);
        }
        return true;
    }

    auto WeaponVirtuals::HolderMaxSpeed() -> float
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return maxSpeed.Call<float>(this);
        }
        auto& weapon = weaponRef->get();
        return static_cast<float>(weapon.executeMaxSpeed(EdictIndex(), player->EdictIndex(), player->fov));
    }

    auto WeaponVirtuals::HolderItemSlot() -> InventorySlot
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            return itemSlot.Call<InventorySlot>(this);
        }
        auto& weapon = weaponRef->get();
        return weapon.getInventorySlot();
    }

    auto WeaponVirtuals::HolderPrimaryAttack() -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            primaryAttack.Call(this);
            return;
        }
        auto& weapon = weaponRef->get();
        weapon.executePrimaryAttack(EdictIndex(), player->EdictIndex(), clip, player->ammo[primary_ammo_type]);
    }

    auto WeaponVirtuals::HolderSecondaryAttack() -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            secondaryAttack.Call(this);
            return;
        }
        auto& weapon = weaponRef->get();
        weapon.executeSecondaryAttack(EdictIndex(), player->EdictIndex(), weapon_state, player->fov);
    }

    auto WeaponVirtuals::HolderReload() -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            reload.Call(this);
            return;
        }
        auto& weapon = weaponRef->get();
        weapon.executeReload(EdictIndex(), player->EdictIndex());
    }

    auto WeaponVirtuals::HolderIdle() -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            idle.Call(this);
            return;
        }
        auto& weapon = weaponRef->get();
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

    //    const Vector &vecOrigin, const Vector &vecLeft, const Vector &vecVelocity, float rotation, int model,
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
    //    WriteShort(model);
    //    WriteByte(soundtype);
    //    WriteByte(25); // life
    //    WriteByte(entityIndex);
    //    MessageEnd();
    //}

    auto WeaponVirtuals::HolderPostFrame() -> void
    {
        const auto weaponRef = weaponModule[vars->impulse];
        if (!weaponRef) {
            postFrame.Call(this);
            return;
        }
        auto& weapon = weaponRef->get();
        auto usableButtons = player->vars->button;
        const auto itemInfo = GetCsPlayerItem()->item_info;
        if (weapon.getPrimaryAttackForward() == -1) {
            usableButtons &= ~IN_ATTACK;
        }
        if (weapon.getSecondaryAttackForward() == -1) {
            usableButtons &= ~IN_ATTACK2;
        }
        if (famas_shoot != 0.f && g_global_vars->time > famas_shoot) {
            weapon.executeFireRemaining(EdictIndex(), player->EdictIndex(), clip, famas_shots_fired);
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
            // if (pszAmmo2() && !m_pPlayer->m_rgAmmo[SecondaryAmmoIndex()]) {
            //     fire_on_empty = TRUE;
            // }
            SecondaryAttack();
            player->vars->button &= ~IN_ATTACK2;
        } else if ((player->vars->button & IN_ATTACK) && next_primary_attack <= 0.f) {
            if ((!clip && itemInfo.ammo1) || (itemInfo.max_clip == WEAPON_NO_CLIP && !player->ammo[primary_ammo_type])) {
                fire_on_empty = true;
            }
            if (player->can_shoot && gameRules->isCanMove()) {
                if (player->vars->water_level == 3 && (itemInfo.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER)) {
                    PlayEmptySound();
                    next_primary_attack = 0.15f;
                } else {
                    PrimaryAttack();
                }
            }
        } else if ((usableButtons & IN_RELOAD) && itemInfo.max_clip != WEAPON_NO_CLIP && !in_reload && next_primary_attack < 0.f) {
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
            fire_on_empty = FALSE;
            if (weapon.getWeaponType() == WeaponType::Secondary) {
                shots_fired = 0;
            } else {
                if (shots_fired > 0 && decrease_shots_fired < g_global_vars->time) {
                    decrease_shots_fired = g_global_vars->time + 0.0225f;
                    shots_fired--;
                    if (shots_fired == 0) {
                        accuracy = weapon.getBaseAccuracy();
                    }
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
}
