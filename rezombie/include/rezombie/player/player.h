#pragma once

#include <messages/engine_message.h>
#include "rezombie/player/long_jump.h"
#include "rezombie/player/player_model.h"
#include "rezombie/player/player_preview.h"
#include "rezombie/player/player_vars.h"
#include "rezombie/weapons/weapons.h"
#include "rezombie/core/base_weapon.h"
#include <cssdk/engine/eiface.h>
#include <cssdk/public/regamedll/cs_player.h>
#include <cssdk/public/utils.h>
#include <optional>

namespace rz::player
{
    using namespace cssdk;
    using namespace rz::weapon;

    constexpr auto NO_MOVE_PLAYER_FLAGS = PLAYER_PREVENT_DUCK | PLAYER_PREVENT_CLIMB | PLAYER_PREVENT_JUMP;

    constexpr auto MAX_RGB_COLORS = 3;
    using RgbColor = std::array<byte, MAX_RGB_COLORS>;

    auto RegisterHooks() -> void;

    class Player {
      private:
        Edict* edict_ = nullptr;
        EntityVars* vars_ = nullptr;
        PlayerBase* base_ = nullptr;
        CsPlayer* cstrike_ = nullptr;
        PlayerPreview* preview_ = nullptr;
        PlayerVars playerVars_ = {};
        float freezeEndTime_ = 0.0f;

        auto SetFootSteps(bool footSteps) -> void;
        auto GiveHealth(int health) -> void;
        auto GiveArmor(int armor, bool helmet = false) -> void;
        auto GiveModel(const PlayerModelHeader& model) -> void;

      public:
        auto init(PlayerBase* base) -> void;
        auto ResetVars() -> void;
        auto ResetExtraJumps() -> void;
        auto ResetLongJump() -> void;
        auto disconnect() -> void;

        auto isValid() const {
            return (IsValidEntity(edict_) && vars_ != nullptr && base_ != nullptr && cstrike_ != nullptr);
        }

        auto id() const {
            return base_->EdictIndex();
        }

        // auto getEntVars() const
        //{
        //     return vars_;
        // }

        auto getEdict() const {
            return edict_;
        }

        // auto getBase() const
        //{
        //     return base_;
        // }

        // auto getCStrike() const
        //{
        //     return cstrike_;
        // }

        auto ShowAppearance() -> void;
        auto SelectAppearance() -> void;
        auto SelectItem(const char* name) -> bool;
        auto FindItemInInventoryByHud(const char* name) const -> PlayerItemBase*;

        auto ChangeClass(int classIndex, Player* attackerUnsafe = nullptr, bool preSpawn = false) -> bool;
        auto ChangeProps(int propsIndex, bool spawn = false) -> bool;
        auto ChangeModel(int modelIndex) -> bool;

        auto GiveWeapon(int weaponIndex, GiveType giveType = GiveType::Append) -> EntityBase*;
        auto DropOrReplace(InventorySlot slot, GiveType giveType) -> void;
        auto CreateBaseWeapon(int weaponIndex, const BaseWeapon& weapon) -> EntityBase*;
        auto GetFreeWeaponId(CrosshairSize crosshairSize) const -> std::optional<std::reference_wrapper<const WeaponId>>;
        auto SendWeaponAnim(int animNumber, int body = 0) -> void;
        auto ResetFovZoom() -> void;

        auto isFrozen() const -> bool;
        auto getFreezeEndTime() const -> float;
        auto Freeze(float freezeTime) -> void;
        auto RemoveFreeze() -> void;

        auto ExtraJump() -> void;

        auto GiveLongJump(int force, int height, float cooldown) -> void;
        auto RemoveLongJump() -> void;
        auto LongJump() -> void;
        auto LongJumpCooldown() -> void;

        auto isPlayableTeam() const {
            const auto team = getTeam();
            return (team == Team::Human || team == Team::Zombie);
        }

        template<typename T = PlayerItemBase, typename F>
        T* forEachItem(InventorySlot slot, const F& func) {
            auto item = base_->player_items[toInt(slot)];
            while (item != nullptr) {
                auto next = item->next;
                if (func(static_cast<T*>(item))) {
                    return static_cast<T*>(item);
                }
                item = next;
            }
            return nullptr;
        }

        operator Edict*() const {
            return edict_;
        }

        operator PlayerBase*() const {
            return base_;
        }

        // inlines?
        auto getHealth() const -> int;
        auto setHealth(int health) -> void;
        auto getMaxHealth() const -> int;
        auto setMaxHealth(int maxHealth) -> void;
        auto getArmor() const -> int;
        auto setArmor(int armor) -> void;
        auto getMaxSpeed() const -> int;
        auto setMaxSpeed(int maxSpeed) -> void;
        auto getGravity() const;
        auto setGravity(float gravity) -> void;
        auto getFlags() const -> int;
        auto setFlags(int flags) -> void;
        auto getDeadFlag() const;
        auto setDeadFlag(DeathState deadFlag) -> void;
        auto setViewModel(unsigned int viewModel) -> void;
        auto setWeaponModel(unsigned int weaponModel) -> void;
        auto setRenderMode(Rendering mode) -> void;
        auto setRenderAmount(int amount) -> void;
        auto setRenderColor(const Color24& color) -> void;
        auto setRenderFx(RenderingFx fx) -> void;
        auto getBody() const -> int;
        auto setBody(int body) -> void;
        auto getEffects() const -> int;
        auto setEffects(int effects) -> void;
        auto getFrags() const -> int;
        auto setFrags(int frags) -> void;
        auto getOrigin() const -> Vector;
        auto setOrigin(Vector origin) -> void;
        auto getAngles() const -> Vector;
        auto setAngles(Vector angles) -> void;
        auto getVelocity() const -> Vector&;
        auto setVelocity(Vector velocity) -> void;
        auto getViewAngle() const -> Vector&;
        auto setViewAngle(Vector viewAngle) -> void;
        auto setFixAngleMode(FixAngleMode fixAngleMod) -> void;
        auto getPunchAngle() const -> Vector&;
        auto setPunchAngle(Vector punchAngle) -> void;
        auto getNetName() const;
        auto getWeapons() const -> int;
        auto setWeapons(int weapons) -> void;
        auto getImpulse() const -> int;
        auto setImpulse(int impulse) -> void;
        auto getWeaponAnim() const -> int;
        auto setWeaponAnim(int weaponAnim) -> void;
        auto isInDuck() const -> qboolean;
        auto getDuckTime() const -> float;
        auto getButton() const -> int;
        auto setButton(int button) -> void;
        auto getOldButtons() const -> int;
        auto setOldButtons(int oldButtons) -> void;
        auto getWaterLevel() const -> int;
        auto setWaterLevel(int waterLevel) -> void;
        auto getIUser1() const -> ObserverMode;
        auto setIUser1(ObserverMode observerMode) -> void;
        auto getIUser3() const -> int;
        auto setIUser3(int iUser3) -> void;

        // inlines?
        auto isAlive() const -> bool;
        auto isBot() const -> bool;
        auto isDormant() const -> bool;
        auto getGunPosition() const -> Vector;
        auto getBloodColor() const -> int;
        auto setBloodColor(int bloodColor) -> void;
        auto getTeam() const -> Team;
        auto getTeamId() const -> int;
        auto setTeam(Team team) -> void;
        auto getAccount() const -> int;
        auto setAccount(int account) -> void;
        auto getMenu() const -> MenuName;
        auto setMenu(MenuName menu) -> void;
        auto getJoiningState() const -> JoinState;
        auto setJoiningState(JoinState joinState) -> void;
        auto getNumSpawns() const -> int;
        auto setNumSpawns(int numSpawns) -> void;
        auto setNextSprayTime(float nextTime) -> void;
        auto getDeaths() const -> int;
        auto setDeaths(int deaths) -> void;
        auto setKevlar(ArmorType armorType) -> void;
        auto getHelmet() const -> bool;
        auto isKilled() const -> bool;
        auto setKilled(bool isKilled) -> void;
        auto isHeadshotKilled() const -> bool;
        auto isJustConnected() const -> bool;
        auto isDisconnected() const -> bool;
        auto setDisconnected(bool isDisconnected) -> void;
        auto getCanShoot() const -> bool;
        auto setCanShoot(int canShoot) -> void;
        auto getHideHud() const -> int;
        auto setHideHud(int hideFlags) -> void;
        auto setNextAttack(float nextAttack) -> void;
        auto getPlayerItems(int slot) const -> PlayerItemBase*;
        auto setPlayerItems(int slot, PlayerItemBase* item) -> void;
        auto getActiveItem() const -> PlayerItemBase*;
        auto setActiveItem(PlayerItemBase* item) -> void;
        auto getLastItem() const -> PlayerItemBase*;
        auto setLastItem(PlayerItemBase* item) -> void;
        auto getAmmo(int ammoIndex) const -> int;
        auto setAmmo(int ammoIndex, int amount) -> void;
        auto getObserverTarget() const -> Edict*;
        auto getButtonLast() const -> int;
        auto setButtonLast(int button) -> void;
        auto getButtonPressed() const -> int;
        auto setButtonPressed(int button) -> void;
        auto getButtonReleased() const -> int;
        auto setButtonReleased(int button) -> void;
        auto setAnimExtension(const char* animExtension) -> void;
        auto ResetMaxSpeed() -> void;
        auto RemovePlayerItem(PlayerItemBase* item) -> qboolean;
        auto RoundRespawn() -> void;

        // inlines?
        auto GiveDefaultItems() -> void;
        auto RemoveAllItems(bool removeSuit = false) -> void;
        auto GiveItem(const char* itemName) -> EntityBase*;
        auto DropPlayerItem(const char* itemName) -> void;
        auto SwitchWeapon(PlayerItemBase* weapon) -> bool;
        auto AddAccount(int amount, RewardType type = RewardType::None, bool trackChange = true) -> void;
        auto SetAnimation(PlayerAnim playerAnim) -> void;
        auto TeamChangeUpdate() -> void;
        auto Reset() -> void;
        auto SpawnEquip() -> void;

        // inlines?
        auto getClass() const -> int;
        auto setClass(int index) -> void;
        auto getSubclass() const -> int;
        auto setSubclass(int index) -> void;
        auto getProps() const -> int;
        auto setProps(int index) -> void;
        auto getModel() const -> int;
        auto setModel(int index) -> void;
        auto getSound() const -> int;
        auto setSound(int index) -> void;
        auto getMelee() const -> int;
        auto setMelee(int index) -> void;
        auto getNightVision() const -> int;
        auto setNightVision(int index) -> void;
        auto getExtraJumpsCount() const -> int;
        auto setExtraJumpsCount(int amount) -> void;
        auto getMaxExtraJumps() const -> int;
        auto setMaxExtraJumps(int amount) -> void;
        auto getLongJumpState() const -> LongJumpState;
        auto setLongJumpState(LongJumpState state) -> void;
        auto getLongJumpNextStateTime() const -> float;
        auto setLongJumpNextStateTime(float nextStateTime) -> void;
        auto getLongJumpForce() const -> int;
        auto setLongJumpForce(int force) -> void;
        auto getLongJumpHeight() const -> int;
        auto setLongJumpHeight(int height) -> void;
        auto getLongJumpCooldown() const -> float;
        auto setLongJumpCooldown(float cooldown) -> void;
        auto getPreview() const -> PlayerPreview*;
    };

    auto WeaponDefaultDeploy(Player& player, PlayerWeaponBase* baseWeapon, int drawAnim, const char* playerAnim)
    -> bool;
    auto WeaponDefaultReload(Player& player, PlayerWeaponBase* baseWeapon, int reloadAnim, float reloadTime) -> bool;
    auto WeaponDefaultShotgunReload(
        Player& player, PlayerWeaponBase* baseWeapon, int reloadAnim, int reloadStartAnim, float reloadDelay,
        float reloadStartDelay, const char* reloadSound1, const char* reloadSound2
    ) -> bool;
    auto WeaponKickBack(
        Player& player, PlayerWeaponBase* baseWeapon, float upBase, float lateralBase, float upModifier,
        float lateralModifier, float upMax, float lateralMax, int directionChange
    ) -> void;
    auto WeaponThrowGrenade(
        Player& player, PlayerWeaponBase* baseWeapon, const Vector& origin, const Vector& velocity, float actionTime
    ) -> cssdk::Grenade*;
}
