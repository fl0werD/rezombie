#pragma once

#include "rezombie/core/base_object.h"
#include "rezombie/weapons/weapons.h"
#include "amxx/api.h"
#include "cssdk/dll/weapon_type.h"
#include <utility>

namespace rz::weapon
{
    using namespace cssdk;
    using namespace amx;
    using namespace amxx;

    class BaseWeapon : public BaseObject {
      private:
        std::string reference_;
        WeaponType weaponType_;
        InventorySlot inventorySlot_;
        std::string viewModel_;
        std::string playerModel_;
        int playerModelBody_ = 0;
        std::string worldModel_;
        int worldModelBody_ = 0;
        std::string hud_;
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
            BaseObject(std::move(handle)),
            reference_(std::move(reference)),
            weaponType_(weaponType),
            inventorySlot_(getDefaultInventorySlot(weaponType_)) {}

        virtual ~BaseWeapon() = default;

        auto getReference() const -> const std::string& { return reference_; }

        auto getViewModel() const -> const std::string& { return viewModel_; }

        auto setViewModel(std::string viewModel) { viewModel_ = std::move(viewModel); }

        auto getPlayerModel() const -> const std::string& { return playerModel_; }

        auto setPlayerModel(std::string playerModel) { playerModel_ = std::move(playerModel); }

        auto getPlayerModelBody() const { return playerModelBody_; }

        auto setPlayerModelBody(int playerModelBody) { playerModelBody_ = playerModelBody; }

        auto getWorldModel() const -> const std::string& { return worldModel_; }

        auto setWorldModel(std::string worldModel) { worldModel_ = std::move(worldModel); }

        auto getWorldModelBody() const { return worldModelBody_; }

        auto setWorldModelBody(int worldModelBody) { worldModelBody_ = worldModelBody; }

        auto getHud() const -> const std::string& { return hud_; }

        auto setHud(std::string hud) { hud_ = std::move(hud); }

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
