/*
 *  Half Life 1 SDK Copyright(c) Valve Corp.
 *
 *  THIS DOCUMENT DESCRIBES A CONTRACT BETWEEN YOU AND VALVE CORPORATION ("Valve").
 *  PLEASE READ IT BEFORE DOWNLOADING OR USING THE HALF LIFE 1 SDK ("SDK"). BY
 *  DOWNLOADING AND/OR USING THE HALF LIFE 1 SDK YOU ACCEPT THIS LICENSE. IF YOU DO
 *  NOT AGREE TO THE TERMS OF THIS LICENSE PLEASE DON'T DOWNLOAD OR USE THE SDK.
 *
 *  You may, free of charge, download and use the SDK to develop a modified Valve
 *  game running on the Half-Life 1 engine. You may distribute your modified Valve
 *  game in source and object code form, but only for free. Terms of use for Valve
 *  games are found in the Steam Subscriber Agreement located here:
 *  http://store.steampowered.com/subscriber_agreement/
 *
 *  You may copy, modify, and distribute the SDK and any modifications you make to
 *  the SDK in source and object code form, but only for free. Any distribution of
 *  this SDK must include this LICENSE file.
 *
 *  Any distribution of the SDK or a substantial portion of the SDK must include
 *  the above copyright notice and the following:
 *
 *    DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 *    DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 *    DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 *    INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 *    NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *    LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 *    FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 *    (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 *    BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 *    LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 *    SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 *  If you would like to use the SDK for a commercial purpose, please contact Valve
 *  at sourceengine@valvesoftware.com.
 */

#pragma once

#include <cssdk/common/vector.h>
#include <cssdk/dll/player.h>
#include <cssdk/public/regamedll/cs_entity.h>

namespace cssdk
{
    enum class WeaponInfiniteAmmoMode
    {
        /**
         * @brief N/D
         */
        Clip = 1,

        /**
         * @brief N/D
         */
        BackpackAmmo
    };

    enum class ProtectionState
    {
        /**
         * @brief N/D
         */
        NoSet = 0,

        /**
         * @brief N/D
         */
        Active,

        /**
         * @brief N/D
         */
        Expired
    };

    class CsPlayer : public CsMonster
    {
    public:
        /**
         * @brief N/D
         */
        [[nodiscard]] virtual bool IsConnected() const = 0;

        /**
         * @brief N/D
         */
        virtual void SetAnimation(PlayerAnim player_anim) = 0;

        /**
         * @brief N/D
         */
        virtual void AddAccount(int amount, RewardType type = RewardType::None, bool track_change = true) = 0;

        /**
         * @brief N/D
         */
        virtual EntityBase* GiveNamedItem(const char* name) = 0;

        /**
         * @brief N/D
         */
        virtual EntityBase* GiveNamedItemEx(const char* name) = 0;

        /**
         * @brief N/D
         */
        virtual void GiveDefaultItems() = 0;

        /**
         * @brief N/D
         */
        virtual void GiveShield(bool deploy = true) = 0;

        /**
         * @brief N/D
         */
        virtual void DropShield(bool deploy = true) = 0;

        /**
         * @brief N/D
         */
        virtual void DropPlayerItem(const char* item_name) = 0;

        /**
         * @brief N/D
         */
        virtual bool RemoveShield() = 0;

        /**
         * @brief N/D
         */
        virtual void RemoveAllItems(bool remove_suit) = 0;

        /**
         * @brief N/D
         */
        virtual bool RemovePlayerItem(const char* item_name) = 0;

        /**
         * @brief N/D
         */
        virtual void SetPlayerModel(bool has_c4) = 0;

        /**
         * @brief N/D
         */
        virtual void SetPlayerModelEx(const char* model_name) = 0;

        /**
         * @brief N/D
         */
        virtual void SetNewPlayerModel(const char* model_name) = 0;

        /**
         * @brief N/D
         */
        virtual void ClientCommand(const char* cmd, const char* arg1 = nullptr, const char* arg2 = nullptr,
                                   const char* arg3 = nullptr) = 0;

        /**
         * @brief N/D
         */
        virtual void SetProgressBarTime(int time) = 0;

        /**
         * @brief N/D
         */
        virtual void SetProgressBarTime2(int time, float time_elapsed) = 0;

        /**
         * @brief N/D
         */
        virtual Edict* EntSelectSpawnPoint() = 0;

        /**
         * @brief N/D
         */
        virtual void SetBombIcon(bool flash = false) = 0;

        /**
         * @brief N/D
         */
        virtual void SetScoreAttribute(PlayerBase* dest) = 0;

        /**
         * @brief N/D
         */
        virtual void SendItemStatus() = 0;

        /**
         * @brief N/D
         */
        virtual void ReloadWeapons(PlayerItemBase* weapon = nullptr, bool force_reload = false, bool force_refill = false) = 0;

        /**
         * @brief N/D
         */
        virtual void ObserverSetMode(int mode) = 0;

        /**
         * @brief N/D
         */
        virtual bool SelectSpawnSpot(const char* ent_class_name, EntityBase*& spot) = 0;

        /**
         * @brief N/D
         */
        virtual bool SwitchWeapon(PlayerItemBase* weapon) = 0;

        /**
         * @brief N/D
         */
        virtual void SwitchTeam() = 0;

        /**
         * @brief N/D
         */
        virtual bool JoinTeam(TeamName team) = 0;

        /**
         * @brief N/D
         */
        virtual void StartObserver(Vector& position, Vector& view_angle) = 0;

        /**
         * @brief N/D
         */
        virtual void TeamChangeUpdate() = 0;

        /**
         * @brief N/D
         */
        virtual void DropSecondary() = 0;

        /**
         * @brief N/D
         */
        virtual void DropPrimary() = 0;

        /**
         * @brief N/D
         */
        virtual bool HasPlayerItem(PlayerItemBase* check_item) = 0;

        /**
         * @brief N/D
         */
        virtual bool HasNamedPlayerItem(const char* item_name) = 0;

        /**
         * @brief N/D
         */
        virtual PlayerItemBase* GetItemById(WeaponId weapon) = 0;

        /**
         * @brief N/D
         */
        virtual PlayerItemBase* GetItemByName(const char* item_name) = 0;

        /**
         * @brief N/D
         */
        virtual void Disappear() = 0;

        /**
         * @brief N/D
         */
        virtual void MakeVip() = 0;

        /**
         * @brief N/D
         */
        virtual bool MakeBomber() = 0;

        /**
         * @brief N/D
         */
        virtual void ResetSequenceInfo() = 0;

        /**
         * @brief N/D
         */
        virtual void StartDeathCam() = 0;

        /**
         * @brief N/D
         */
        virtual bool RemovePlayerItemEx(const char* item_name, bool remove_ammo) = 0;

        /**
         * @brief N/D
         */
        virtual void SetSpawnProtection(float protection_time) = 0;

        /**
         * @brief N/D
         */
        virtual void RemoveSpawnProtection() = 0;

        /**
         * @brief N/D
         */
        virtual bool HintMessageEx(const char* message, float duration = 6.F, bool display_if_player_dead = false,
                                   bool override = false) = 0;

        /**
         * @brief N/D
         */
        virtual void Reset();

        /**
         * @brief N/D
         */
        virtual void OnSpawnEquip(bool addDefault = true, bool equipGame = true);

        /**
         * @brief N/D
         */
        virtual void SetScoreboardAttributes(PlayerBase* destination = nullptr);

        /**
         * @brief N/D
         */
        char model[32]{};

        /**
         * @brief N/D
         */
        bool force_show_menu{};

        /**
         * @brief N/D
         */
        float respawn_pending{};

        /**
         * @brief N/D
         */
        float spawn_protection_end_time{};

        /**
         * @brief N/D
         */
        Vector old_viewing_angle{};

        /**
         * @brief N/D
         */
        WeaponInfiniteAmmoMode infinite_ammo_mode{};

        /**
         * @brief N/D
         */
        int weapon_infinite_ids{};

        /**
         * @brief N/D
         */
        bool can_shoot_override{};

        /**
         * @brief N/D
         */
        bool game_forcing_respawn{};

        /**
         * @brief N/D
         */
        bool auto_bunny_hopping{};

        /**
         * @brief N/D
         */
        bool mega_bunny_jumping{};

        /**
         * @brief N/D
         */
        bool plant_c4_anywhere{};

        /**
         * @brief N/D
         */
        [[nodiscard]] PlayerBase* GetPlayerBase() const
        {
            return reinterpret_cast<PlayerBase*>(this->containing_entity);
        }

        /**
         * @brief N/D
         */
        [[nodiscard]] ProtectionState GetProtectionState() const
        {
            // No protection set.
            if (spawn_protection_end_time <= 0.0f) {
                return ProtectionState::NoSet;
            }

            // Check if end time of protection isn't expired yet.
            if (spawn_protection_end_time >= g_global_vars->time) {
                return ProtectionState::Active;
            }

            // Has expired.
            return ProtectionState::Expired;
        }
    };
}
