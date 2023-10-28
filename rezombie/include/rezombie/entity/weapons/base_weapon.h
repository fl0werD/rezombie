#pragma once

#include "rezombie/core/object.h"
#include "rezombie/weapons/weapons.h"
#include <cssdk/dll/weapon_type.h>
#include <core/strings.h>
#include <amxx/api.h>
#include <utility>

namespace rz
{
    using namespace cssdk;
    using namespace core;
    using namespace amx;
    using namespace amxx;

    class BaseWeapon : public Object {
        std::string reference_;
        WeaponType weaponType_;
        InventorySlot inventorySlot_;
        int viewModelId_{};
        int playerModelId_{};
        int worldModelId_{};
        std::string hud_;
        std::string hudPath_{};
        int maxClip_ = WEAPON_NO_CLIP;
        int maxAmmo_ = 0;
        int weight_ = 0;
        int flags_ = 0;
        int deployForward_ = -1;
        int holsterForward_ = -1;
        int maxSpeedForward_ = -1;
        int primaryAttackForward_ = -1;
        int secondaryAttackForward_ = -1;
        int reloadForward_ = -1;
        int idleForward_ = -1;

        auto setHudPath(const std::string& hud) -> void {
            const auto hudPath = str::Format("sprites/%s.txt", hud);
            if (!isFileExists(hudPath)) {
                return;
            }
            hudPath_ = hudPath;
        }

        static auto getDefaultInventorySlot(WeaponType weaponType) -> InventorySlot {
            switch (weaponType) {
                case WeaponType::Primary:
                    return InventorySlot::Primary;
                case WeaponType::Secondary:
                    return InventorySlot::Pistol;
                case WeaponType::Melee:
                    return InventorySlot::Knife;
                case WeaponType::Grenade:
                    return InventorySlot::Grenade;
                case WeaponType::Extra:
                    return InventorySlot::C4;
                default:
                    return InventorySlot::None;
            }
        }

      public:
        BaseWeapon(std::string handle, std::string reference, WeaponType weaponType) :
            Object(std::move(handle)),
            reference_(std::move(reference)),
            weaponType_(weaponType),
            inventorySlot_(getDefaultInventorySlot(weaponType_)) {
            setHudPath(getHud());
        }

        virtual ~BaseWeapon() = default;

        auto getReference() const -> const std::string& { return reference_; }

        auto getViewModel() const -> int { return viewModelId_; }

        auto setViewModel(int viewModelId) { viewModelId_ = viewModelId; }

        auto getPlayerModel() const -> int { return playerModelId_; }

        auto setPlayerModel(int playerModelId) { playerModelId_ = playerModelId; }

        auto getWorldModel() const -> int { return worldModelId_; }

        auto setWorldModel(int worldModelId) { worldModelId_ = worldModelId; }

        auto getHud() const -> const std::string& { return hud_; }

        auto setHud(std::string hud) {
            hud_ = std::move(hud);
            setHudPath(getHud());
        }

        auto getHudPath() const -> const std::string& { return hudPath_; }

        auto getMaxClip() const { return maxClip_; }

        auto setMaxClip(int maxClip) { maxClip_ = maxClip; }

        auto getMaxAmmo() const { return maxAmmo_; }

        auto setMaxAmmo(int maxAmmo) { maxAmmo_ = maxAmmo; }

        auto getInventorySlot() const { return inventorySlot_; }

        auto setInventorySlot(InventorySlot inventorySlot) { inventorySlot_ = inventorySlot; }

        auto getWeight() const { return weight_; }

        auto setWeight(int weight) { weight_ = weight; }

        auto getFlags() const { return flags_; }

        auto setFlags(int flags) { flags_ = flags; }

        auto getWeaponType() const { return weaponType_; }

        auto setWeaponType(WeaponType weaponType) { weaponType_ = weaponType; }

        auto getDeployForward() const { return deployForward_; }

        auto setDeployForward(int deployForward) { deployForward_ = deployForward; }

        auto executeDeploy(int weapon, int player) const -> bool {
            if (getDeployForward() == -1) {
                return false;
            }
            return ExecuteForward(getDeployForward(), weapon, player);
        }

        auto getHolsterForward() const { return holsterForward_; }

        auto setHolsterForward(int holsterForward) { holsterForward_ = holsterForward; }

        auto executeHolster(int weapon, int player) const -> bool {
            if (getHolsterForward() == -1) {
                return false;
            }
            ExecuteForward(getHolsterForward(), weapon, player);
            return true;
        }

        auto getMaxSpeedForward() const { return maxSpeedForward_; }

        auto setMaxSpeedForward(int maxSpeedForward) { maxSpeedForward_ = maxSpeedForward; }

        auto executeMaxSpeed(int weapon, int player, int fov) const -> int {
            if (getMaxSpeedForward() == -1) {
                return 0;
            }
            return ExecuteForward(getMaxSpeedForward(), weapon, player, fov);
        }

        auto getPrimaryAttackForward() const { return primaryAttackForward_; }

        auto setPrimaryAttackForward(int primaryAttackForward) { primaryAttackForward_ = primaryAttackForward; }

        auto executePrimaryAttack(int weapon, int player, int clip, int ammo) const -> bool {
            if (getPrimaryAttackForward() == -1) {
                return false;
            }
            ExecuteForward(getPrimaryAttackForward(), weapon, player, clip, ammo);
            return true;
        }

        auto getSecondaryAttackForward() const { return secondaryAttackForward_; }

        auto setSecondaryAttackForward(int secondaryAttackForward) { secondaryAttackForward_ = secondaryAttackForward; }

        auto executeSecondaryAttack(int weapon, int player, int weaponState, int fov) const -> bool {
            if (getSecondaryAttackForward() == -1) {
                return false;
            }
            ExecuteForward(getSecondaryAttackForward(), weapon, player, weaponState, fov);
            return true;
        }

        auto getReloadForward() const { return reloadForward_; }

        auto setReloadForward(int reloadForward) { reloadForward_ = reloadForward; }

        auto executeReload(int weapon, int player) const -> bool {
            if (getReloadForward() == -1) {
                return false;
            }
            ExecuteForward(getReloadForward(), weapon, player);
            return true;
        }

        auto getIdleForward() const { return idleForward_; }

        auto setIdleForward(int idleForward) { idleForward_ = idleForward; }

        auto executeIdle(int weapon, int player, float idleTime) const -> bool {
            if (getIdleForward() == -1) {
                return false;
            }
            ExecuteForward(getIdleForward(), weapon, player, idleTime);
            return true;
        }
    };
}
