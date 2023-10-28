#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include "rezombie/weapons/weapons.h"
#include "rezombie/entity/weapons/weapon.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/messages/engine_message.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/player/modules/player_props.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz
{
    using namespace core;
    using namespace cssdk;
    using namespace metamod;
    using namespace metamod::gamedll;

    auto Player::DropOrReplace(InventorySlot slot, GiveType giveType) -> void {
        if (giveType == GiveType::Append) {
            return;
        }
        forEachItem(slot, [&](PlayerItem* item) {
            if (giveType == GiveType::Replace) {
                setWeapons(getWeapons() & ~(1 << (item->vars->i_user1)));
                RemovePlayerItem(item);
                item->Kill();
            } else if (giveType == GiveType::DropAndReplace) {
                DropPlayerItem(item);
            }
            return false;
        });
    }

    auto Player::DropPlayerItem(PlayerItem* item) -> EntityBase* {
        if (!item) {
            return nullptr;
        }
        auto& player = Players[item->vars->owner];
        const auto propsRef = Props[player.getProps()];
        if (propsRef) {
            const auto& props = propsRef->get();
            if (!props.getWeaponsInteraction()) {
                sendTextMsg(player, HudPrint::Center, "#Weapon_Cannot_Be_Dropped");
                return nullptr;
            }
        }
        if (!item->CanDrop()) {
            sendTextMsg(player, HudPrint::Center, "#Weapon_Cannot_Be_Dropped");
            return nullptr;
        }
        if (item->player) {
            if (item->player->active_item == item) {
                item->Holster();
            }
            if (!item->player->RemovePlayerItem(item)) {
                return nullptr;
            }
        }
        setWeapons(getWeapons() & ~(1 << toInt(item->id)));
        if ((getWeapons() & ~(1 << WEAPON_SUIT)) == 0) {
            setHideHud(getHideHud() | HIDE_HUD_WEAPONS);
        }
        gameRules->GetNextBestWeapon(*this, item);

        MakeVectors(getAngles());
        item->vars->origin = getOrigin() + g_global_vars->vec_forward * 10;
        item->vars->angles = getAngles();
        item->vars->angles.x = 0;
        item->vars->angles.z = 0;
        item->vars->velocity = g_global_vars->vec_forward * 400;

        item->vars->move_type = MoveTypeEntity::Toss;
        item->vars->solid = SolidType::Trigger;
        item->vars->aim_entity = nullptr;
        item->vars->effects &= ~EF_NO_DRAW;

        const auto weaponRef = Weapons[item->vars->impulse];
        if (weaponRef) {
            const auto& weapon = weaponRef->get();
            rz::setModel(item->vars, weapon.getWorldModel());
        }
        //pWeapon->pev->spawnflags |= SF_NORESPAWN;
        //pWeapon->pev->owner = ENT(pev);
        item->vars->next_think = g_global_vars->time + 5;
        //item->SetThink(WeaponBox::Killed());
        //pWeapon->SetThink(nullptr);
        //pWeapon->SetTouch(nullptr);
        item->player = nullptr;

        item->SetThink(&PlayerItem::RemoveThink);
        item->SetTouch(&PlayerItem::TouchWeapon);
        return item;
    }

    auto Player::GetFreeWeaponId(CrosshairSize crosshairSize) const -> std::optional<WeaponId> {
        std::vector<WeaponId> freeIds;
        switch (crosshairSize) {
            case CrosshairSize::None: {
                freeIds = {
                    WeaponId::Scout,
                    WeaponId::Sg550,
                    WeaponId::Awp,
                    WeaponId::G3Sg1,
                };
                break;
            }
            case CrosshairSize::Size3: {
                freeIds = {
                    WeaponId::Aug,
                };
                break;
            }
            case CrosshairSize::Size4: {
                freeIds = {
                    WeaponId::Elite,
                    WeaponId::Galil,
                    WeaponId::Famas,
                    WeaponId::M4A1,
                    WeaponId::Ak47,
                };
                break;
            }
            case CrosshairSize::Size5: {
                freeIds = {
                    WeaponId::Sg552,
                };
                break;
            }
            case CrosshairSize::Size6: {
                freeIds = {
                    WeaponId::C4,
                    WeaponId::Ump45,
                    WeaponId::Mp5Navy,
                    WeaponId::M249,
                };
                break;
            }
            case CrosshairSize::Size7: {
                freeIds = {
                    WeaponId::Tmp,
                    WeaponId::P90,
                    /*knife*/
                };
                break;
            }
            case CrosshairSize::Size8: {
                freeIds = {
                    WeaponId::P228,
                    WeaponId::HeGrenade,
                    WeaponId::SmokeGrenade,
                    WeaponId::Fiveseven,
                    WeaponId::Usp,
                    WeaponId::Glock18,
                    WeaponId::M3,
                    WeaponId::FlashBang,
                    WeaponId::Deagle,
                };
                break;
            }
            case CrosshairSize::Size9: {
                freeIds = {
                    WeaponId::Xm1014,
                    WeaponId::Mac10,
                };
                break;
            }
        }
        for (auto slot = 0; slot < MAX_ITEM_TYPES; ++slot) {
            auto item = getPlayerItems(slot);
            while (item != nullptr) {
                if (item->vars->i_user1) {
                    freeIds.erase(
                        std::remove(
                            freeIds.begin(),
                            freeIds.end(),
                            static_cast<WeaponId>(item->vars->i_user1)
                        ),
                        freeIds.end()
                    );
                }
                item = item->next;
            }
        }
        if (freeIds.empty()) {
            return std::nullopt;
        }
        return freeIds[0];
    }

    auto Player::CreateBaseWeapon(int weaponIndex, BaseWeapon& baseWeapon) -> EntityBase* {
        auto freeId = toInt(WeaponId::Knife);
        if (baseWeapon.getWeaponType() != WeaponType::Melee) {
            const auto weapon = dynamic_cast<Weapon*>(&baseWeapon);
            if (weapon == nullptr) {
                return nullptr;
            }
            const auto freeWeaponId = GetFreeWeaponId(weapon->getCrosshairSize());
            if (!freeWeaponId) {
                return nullptr;
            }
            freeId = toInt(*freeWeaponId);
        }
        const auto edict = UTIL_CreateNamedEntity(AllocString(baseWeapon.getReference().c_str()));
        if (!IsValidEntity(edict)) {
            return nullptr;
        }
        edict->vars.impulse = weaponIndex;
        edict->vars.net_name = AllocString(baseWeapon.getName().c_str());
        edict->vars.i_user1 = freeId;
        Spawn(edict);
        Touch(edict, *this);
        auto entity = EntityPrivateData<EntityBase>(edict);
        if (!IsValidEntity(entity->vars->owner) || entity->vars->owner != *this) {
            entity->vars->flags |= FL_KILL_ME;
            return nullptr;
        }
        setAmmo(edict->vars.i_user1, baseWeapon.getMaxAmmo());
        return entity;
    }

    auto Player::GiveWeapon(int weaponIndex, GiveType giveType) -> EntityBase* {
        auto weaponRef = Weapons[weaponIndex];
        if (!weaponRef) {
            return nullptr;
        }
        auto& weapon = weaponRef->get();
        DropOrReplace(weapon.getInventorySlot(), giveType);
        return CreateBaseWeapon(weaponIndex, weapon);
    }

    auto Player::SendWeaponAnim(int animNumber, int body) -> void {
        setWeaponAnim(animNumber);
        netWeaponAnim(*this, animNumber, body);
        Players.forEachConnected([&](auto& player) {
            if (player.getObserverMode() != ObserverMode::InEye) {
                return;
            }
            if (player.getObserverTarget() != *this) {
                return;
            }
            player.setWeaponAnim(animNumber);
            netWeaponAnim(player, animNumber, body);
        });
    }

    auto WeaponDefaultDeploy(
        Player& player,
        PlayerWeaponBase* baseWeapon,
        int drawAnim,
        const char* playerAnimation
    ) -> bool {
        if (!baseWeapon->CanDeploy()) {
            return false;
        }
        const auto weaponRef = Weapons[baseWeapon->vars->impulse];
        if (weaponRef) {
            const auto& weapon = weaponRef->get();
            const auto& viewModelRef = Models[weapon.getViewModel()];
            if (viewModelRef) {
                const auto viewModel = viewModelRef->get();
                player.setViewModel(AllocString(viewModel.getPath().c_str()));
            }
            player.setWeaponModel(0);
            setModel(baseWeapon->vars, weapon.getPlayerModel());
        }
        baseWeapon->time_weapon_idle = 1.5f;
        baseWeapon->last_fire_time = 0.f;
        baseWeapon->decrease_shots_fired = g_global_vars->time;
        player.setAnimExtension(playerAnimation);
        player.setNextAttack(0.75);
        player.ResetFovZoom();
        player.SendWeaponAnim(drawAnim);
        return true;
    }

    auto WeaponDefaultReload(
        Player& player,
        PlayerWeaponBase* baseWeapon,
        int reloadAnim,
        float reloadTime
    ) -> bool {
        const auto ammo = player.getAmmo(baseWeapon->primary_ammo_type);
        if (ammo <= 0) {
            return false;
        }
        const auto itemInfo = baseWeapon->GetCsPlayerItem()->item_info;
        const auto difference = std::min(itemInfo.max_clip - baseWeapon->clip, ammo);
        if (!difference) {
            return false;
        }
        player.setNextAttack(reloadTime);
        // ReloadSound();
        player.SendWeaponAnim(reloadAnim);
        baseWeapon->in_reload = true;
        baseWeapon->time_weapon_idle = reloadTime + 0.5f;
        return true;
    }

    auto WeaponDefaultShotgunReload(
        Player& player,
        PlayerWeaponBase* baseWeapon,
        int reloadAnim,
        int reloadStartAnim,
        float reloadDelay,
        float reloadStartDelay,
        const char* reloadSound1,
        const char* reloadSound2
    ) -> bool {
        const auto ammo = player.getAmmo(baseWeapon->primary_ammo_type);
        const auto itemInfo = baseWeapon->GetCsPlayerItem()->item_info;
        if (ammo <= 0 || baseWeapon->clip == itemInfo.max_clip) {
            return false;
        }
        if (baseWeapon->next_primary_attack > 0.0) {
            return false;
        }
        if (baseWeapon->in_special_reload == 0) {
            player.SetAnimation(PlayerAnim::Reload);
            player.SendWeaponAnim(reloadStartAnim);
            player.setNextAttack(reloadStartDelay);
            baseWeapon->in_special_reload = 1;
            baseWeapon->next_primary_attack = reloadStartDelay;
            baseWeapon->next_secondary_attack = reloadStartDelay;
            baseWeapon->time_weapon_idle = reloadStartDelay;
        } else if (baseWeapon->in_special_reload == 1) {
            if (baseWeapon->time_weapon_idle > 0.0) {
                return false;
            }
            baseWeapon->in_special_reload = 2;
            const char* reloadSound = nullptr;
            if (reloadSound1 && reloadSound2) {
                reloadSound = RandomLong(0, 1) ? reloadSound1 : reloadSound2;
            } else if (reloadSound1) {
                reloadSound = reloadSound1;
            } else if (reloadSound2) {
                reloadSound = reloadSound2;
            }
            if (reloadSound && reloadSound[0] != '\0') {
                EmitSound(player, SoundChannel::Item, reloadSound, VOL_NORM, ATTN_NORM, 0, 85 + RandomLong(0, 31));
            }
            player.SendWeaponAnim(reloadAnim);
            baseWeapon->next_reload = reloadDelay;
            baseWeapon->time_weapon_idle = reloadDelay;
        } else if (baseWeapon->time_weapon_idle <= 0.0) {
            baseWeapon->in_special_reload = 1;
            baseWeapon->clip++;
            // if (refill_bpammo_weapons.value < 3.0f)
            {
                player.setAmmo(baseWeapon->primary_ammo_type, ammo - 1);
            }
        }
        return true;
    }

    auto WeaponKickBack(
        Player& player,
        PlayerWeaponBase* baseWeapon,
        float upBase,
        float lateralBase,
        float upModifier,
        float lateralModifier,
        float upMax,
        float lateralMax,
        int directionChange
    ) -> void {
        float kickUp;
        float kickLateral;
        if (baseWeapon->shots_fired == 1) {
            kickUp = upBase;
            kickLateral = lateralBase;
        } else {
            kickUp = static_cast<float>(baseWeapon->shots_fired) * upModifier + upBase;
            kickLateral = static_cast<float>(baseWeapon->shots_fired) * lateralModifier + lateralBase;
        }
        auto& punchAngle = player.getPunchAngle();
        punchAngle.x -= kickUp;
        if (punchAngle.x < -upMax) {
            punchAngle.x = -upMax;
        }
        if (baseWeapon->direction) {
            punchAngle.y += kickLateral;
            if (punchAngle.y > lateralMax) {
                punchAngle.y = lateralMax;
            }
        } else {
            punchAngle.y -= kickLateral;
            if (punchAngle.y < -lateralMax) {
                punchAngle.y = -lateralMax;
            }
        }
        player.setPunchAngle(punchAngle);
        if (!RandomLong(0, directionChange)) {
            baseWeapon->direction = !baseWeapon->direction;
        }
    }

    auto WeaponThrowGrenade(
        Player& player,
        PlayerWeaponBase* baseWeapon,
        const Vector& origin,
        const Vector& velocity,
        float actionTime
    ) -> Grenade* {
        static auto grenadeClassName = AllocString("grenade");
        auto grenadeEntity = UTIL_CreateNamedEntity(grenadeClassName);
        auto grenade = EntityPrivateData<Grenade>(grenadeEntity);
        grenade->Spawn();
        const int weaponId = baseWeapon->vars->impulse;
        const auto weaponRef = Weapons[weaponId];
        if (weaponRef) {
            const auto& weapon = weaponRef->get();
            const auto worldModelRef = Models[weapon.getWorldModel()];
            if (worldModelRef) {
                const auto& worldModel = worldModelRef->get();
                grenade->vars->model_index = worldModel.getPrecacheId();
            }
        }
        grenade->vars->impulse = weaponId;
        grenade->vars->velocity = velocity;
        grenade->vars->angles = player.getAngles();
        grenade->vars->owner = player;
        grenade->vars->damage_time = g_global_vars->time + actionTime;
        grenade->vars->next_think = g_global_vars->time + 0.1f;
        grenade->vars->sequence = RandomLong(3, 6);
        grenade->vars->frame_rate = 1.f;
        grenade->vars->anim_time = g_global_vars->time; // not original
        grenade->vars->gravity = 0.55f;
        grenade->vars->friction = 0.7f;
        grenade->team = static_cast<TeamName>(player.getTeam());
        SetOrigin(grenadeEntity, origin);
        return grenade;
    }
}
