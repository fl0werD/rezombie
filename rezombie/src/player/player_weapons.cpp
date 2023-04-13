#include <messages/engine_message.h>
#include "rezombie/modules/weapon.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include "rezombie/weapons/weapons.h"
#include "rezombie/entity/weapon.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz::player
{
    using namespace core;
    using namespace cssdk;
    using namespace metamod;
    using namespace metamod::gamedll;
    using namespace message;

    auto Player::DropOrReplace(InventorySlot slot, GiveType giveType) -> void {
        if (giveType == GiveType::Append) {
            return;
        }
        forEachItem(slot, [&](PlayerItemBase* item) {
            if (giveType == GiveType::Replace) {
                setWeapons(getWeapons() & ~(1 << (item->vars->i_user1)));
                RemovePlayerItem(item);
                item->Kill();
            } else if (giveType == GiveType::DropAndReplace) {
                DropPlayerItem(item->vars->class_name.CStr());
            }
            return false;
        });
    }

    auto Player::GetFreeWeaponId(CrosshairSize crosshairSize) const -> std::optional<std::reference_wrapper<const WeaponId>> {
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
        const auto copiedWeaponId = freeIds[0];
        return {copiedWeaponId};
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
            freeId = toInt(freeWeaponId->get());
        }
        const auto edict = CreateNamedEntity(AllocString(baseWeapon.getReference().c_str()));
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
        auto weaponRef = weaponModule[weaponIndex];
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
        players.forEachConnected([&](auto& player) {
            if (player.getIUser1() != ObserverMode::InEye) {
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
        const char* playerAnim
    ) -> bool {
        if (!baseWeapon->CanDeploy()) {
            return false;
        }
        const auto weaponRef = weaponModule[baseWeapon->vars->impulse];
        if (weaponRef) {
            const auto& weapon = weaponRef->get();
            player.setViewModel(AllocString(weapon.getViewModel().c_str()));
            player.setWeaponModel(AllocString(weapon.getPlayerModel().c_str()));
        }
        baseWeapon->time_weapon_idle = 1.5f;
        baseWeapon->last_fire_time = 0.f;
        baseWeapon->decrease_shots_fired = g_global_vars->time;
        player.setAnimExtension(playerAnim);
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
        using namespace core::regamedll_api::detail;
        auto grenadeEntity = regamedll_funcs->create_named_entity2(AllocString("grenade"));
        auto grenade = EntityPrivateData<cssdk::Grenade>(grenadeEntity);
        grenade->Spawn();
        SetOrigin(grenadeEntity, origin);
        const int weaponIndex = baseWeapon->vars->impulse;
        const auto weaponRef = weaponModule[weaponIndex];
        if (weaponRef) {
            auto& weapon = weaponRef->get();
            SetModel(grenadeEntity, weapon.getWorldModel().c_str());
        }
        grenade->vars->impulse = weaponIndex;
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
        return grenade;
    }
}
