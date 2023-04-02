#include "rezombie/player/player.h"
#include "rezombie/amxmodx/player.h"
#include "rezombie/amxmodx/player_class.h"
#include <messages/user_message.h>
#include "rezombie/modules/player_class.h"
#include "rezombie/modules/player_model.h"
#include "rezombie/modules/player_sound.h"
#include "rezombie/modules/weapon.h"
#include "rezombie/player/player_preview.h"
#include "rezombie/player/players.h"
#include "rezombie/util.h"
#include <core/menu.h>
#include <metamod/engine.h>
#include <metamod/gamedll.h>
#include <mhooks/reapi.h>

namespace rz::player
{
    using namespace core;
    using namespace cssdk;
    using namespace metamod;
    using namespace message;

    auto Player::init(PlayerBase* base) -> void
    {
        edict_ = base->GetEdict();
        vars_ = &edict_->vars;
        base_ = base;
        cstrike_ = base->GetCsPlayer();
        ResetVars();
    }

    auto Player::disconnect() -> void
    {
        edict_ = nullptr;
        vars_ = nullptr;
        base_ = nullptr;
        cstrike_ = nullptr;
    }

    auto Player_Spawn(const ReGamePlayerSpawnMChain& chain, PlayerBase* base) -> void
    {
        auto& player = players[base];
        if (player.isJustConnected() && player.getJoiningState() == JoinState::PickingTeam) {
            chain.CallNext(base);
            return;
        }
        // if (!player.isPlayableTeam()) { // maybe no need
        //     chain.CallNext(base);
        //     return;
        // }
        if (gameRules->isCanMove()) {
            player.setIUser3(player.getIUser3() & ~NO_MOVE_PLAYER_FLAGS);
        }
        auto newClass = gameRules->getDefaultPlayerClass(Team::Human);
        if (gameRules->getGameState() == GameState::Playing && gameRules->getRoundState() == RoundState::Playing) {
            newClass = gameRules->getDefaultPlayerClass(Team::Zombie);
        }
        if (player.getClass() != newClass) {
            player.setKilled(true);
            player.setClass(newClass);
        }
        player.ChangeClass(player.getClass(), &player, true);
        const auto body = player.getBody();
        chain.CallNext(base);
        player.setBody(body);
        player.setHideHud(player.getHideHud() | HIDE_HUD_TIMER);
    }

    auto Player_PreThink(const ReGamePlayerPreThinkMChain& chain, PlayerBase* base)
    {
        chain.CallNext(base);
        auto& player = players[base];
        if (player.getMenu() == MenuName::ChooseAppearance) {
            // player.getPreview()->UpdatePosition(player.getGunPosition(), player.getViewAngle());
        }
        if (!player.isAlive()) {
            return;
        }
        if (player.isFrozen() && player.getFreezeEndTime() <= g_global_vars->time) {
            player.RemoveFreeze();
        }
        player.LongJumpCooldown();
    }

    auto Player_AddPlayerItem(const ReGamePlayerAddPlayerItemMChain&, PlayerBase* base, PlayerItemBase* item)
      -> qboolean
    {
        // add can interact?
        const auto itemSlot = item->GetCsPlayerItem()->item_info.slot;
        auto& player = players[base];
        auto insert = player.getPlayerItems(itemSlot);
        while (insert != nullptr) {
            if (insert->vars->impulse == item->vars->impulse) {
                if (item->AddDuplicate(insert)) {
                    item->Kill();
                }
                return false;
            }
            insert = insert->next;
        }
        if (!item->AddToPlayer(player)) {
            return false;
        }
        item->next = player.getPlayerItems(itemSlot);
        player.setPlayerItems(itemSlot, item);
        if (gameRules->ShouldSwitchWeapon(player, item)) {
            player.SwitchWeapon(item);
        }
        player.setHideHud(player.getHideHud() & ~HIDE_HUD_WEAPONS);
        return true;
    }

    auto Player_RemovePlayerItem(const ReGamePlayerRemovePlayerItemMChain&, PlayerBase* base, PlayerItemBase* item)
      -> qboolean
    {
        auto& player = players[base];
        if (player.getActiveItem() == item) {
            player.ResetFovZoom();
            player.ResetMaxSpeed();
            item->vars->next_think = 0.f;
            item->SetThink(nullptr);
            player.setActiveItem(nullptr);
            player.setViewModel(0);
            player.setWeaponModel(0);
        }
        if (player.getLastItem() == item) {
            player.setLastItem(nullptr);
        }
        const auto itemSlot = item->GetCsPlayerItem()->item_info.slot;
        auto prev = player.getPlayerItems(itemSlot);
        if (prev == item) {
            player.setPlayerItems(itemSlot, item->next);
            return true;
        }
        while (prev != nullptr && prev->next != item) {
            prev = prev->next;
        }
        if (prev != nullptr) {
            prev->next = item->next;
            return true;
        }
        return false;
    }

    auto Player_SpawnEquip(const ReGamePlayerSpawnEquipMChain&, PlayerBase* base, bool, bool) -> void
    {
        auto& player = players[base];
        player.ChangeProps(player.getProps(), true);
        if (!player.isKilled()) {
            return;
        }
        player.GiveDefaultItems();
    }

    auto Player_GiveDefaultItems(const ReGamePlayerGiveDefaultItemsMChain&, PlayerBase* base) -> void
    {
        auto& player = players[base];
        player.RemoveAllItems();
        player.GiveMelee(player.getMelee());
        amxxPlayer.PlayerGiveDefaultItems(player);
    }

    auto Player_Jump(const ReGamePlayerJumpMChain& chain, PlayerBase* base) -> void
    {
        chain.CallNext(base);
        auto& player = players[base];
        if (player.getIUser3() & NO_MOVE_PLAYER_FLAGS) {
            return;
        }
        player.ExtraJump();
        player.LongJump();
    }

    auto Player_ResetMaxSpeed(const ReGamePlayerResetMaxSpeedMChain& chain, PlayerBase* base) -> void
    {
        chain.CallNext(base);
        auto& player = players[base];
        if (!player.isAlive()) {
            return;
        }
        if (!gameRules->isCanMove() || player.isFrozen()) {
            player.setMaxSpeed(1);
            return;
        }
        const auto activeItem = player.getActiveItem();
        if (activeItem != nullptr) {
            player.setMaxSpeed(static_cast<int>(activeItem->GetMaxSpeed()));
        } else {
            player.setMaxSpeed(240);
        }
    }

    auto Player_Pain(const ReGamePlayerPainMChain& chain, PlayerBase* base, HitBoxGroup lastHitGroup, bool hasArmour)
      -> void
    {
        const auto& player = players[base];
        const auto soundRef = playerSoundModule[player.getSound()];
        if (!soundRef) {
            chain.CallNext(base, lastHitGroup, hasArmour);
            return;
        }
        const auto& sound = soundRef->get();
        if (lastHitGroup == HitBoxGroup::Head) {
            if (player.getHelmet()) {
                UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::ArmoredHead));
            } else {
                UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::Head));
            }
        } else {
            if (lastHitGroup != HitBoxGroup::LeftLeg && lastHitGroup != HitBoxGroup::RightLeg) {
                if (hasArmour) {
                    UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::ArmoredBody));
                    return;
                }
            }
            UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::Body));
        }
    }

    auto Player_DeathSound(const ReGamePlayerDeathSoundMChain& chain, PlayerBase* base) -> void
    {
        const auto& player = players[base];
        const auto soundRef = playerSoundModule[player.getSound()];
        if (!soundRef) {
            chain.CallNext(base);
            return;
        }
        const auto& sound = soundRef->get();
        UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::Death));
    }

    auto Player_JoiningThink(const ReGamePlayerJoiningThinkMChain& chain, PlayerBase* base) -> void
    {
        const auto& player = players[base];
        chain.CallNext(base);
        // if (player.getJoiningState() == JoinState::Joined || player.getJoiningState() == JoinState::GetIntoGame)
        // {
        //     chain.CallNext(base);
        //     return;
        // }
        //  if (m_pIntroCamera && gpGlobals->time >= m_fIntroCamTime) {
        //      // find the next another camera
        //      m_pIntroCamera = UTIL_FindEntityByClassname(m_pIntroCamera, "trigger_camera");

        //    // could not find, go back to the start
        //    if (!m_pIntroCamera) {
        //        m_pIntroCamera = UTIL_FindEntityByClassname(nullptr, "trigger_camera");
        //    }

        //    CBaseEntity* Target = UTIL_FindEntityByTargetname(nullptr, STRING(m_pIntroCamera->pev->target));
        //    if (Target) {
        //        Vector vecAngles = UTIL_VecToAngles((Target->pev->origin -
        //        m_pIntroCamera->pev->origin).Normalize());

        //        vecAngles.x = -vecAngles.x;
        //        UTIL_SetOrigin(pev, m_pIntroCamera->pev->origin);

        //        pev->angles = vecAngles;
        //        pev->v_angle = pev->angles;

        //        pev->velocity = g_vecZero;
        //        pev->punchangle = g_vecZero;

        //        pev->fixangle = 1;
        //        pev->view_ofs = g_vecZero;
        //        m_fIntroCamTime = gpGlobals->time + 6;
        //    } else {
        //        m_pIntroCamera = nullptr;
        //    }
        //}
    }

    auto InternalCommand(const ReGameInternalCommandMChain& chain, Edict* client, const char* command, const char* arg1)
      -> void
    {
        auto& player = players[client];
        if (str::Equals(command, "menuselect")) {
            if (player.getMenu() == MenuName::ChooseAppearance) {
                player.SelectAppearance();
                return;
            }
        } else if (str::Equals(command, "chooseteam")) {
            return;
        } else {
            if (player.SelectItem(command)) {
                return;
            }
        }
        chain.CallNext(client, command, arg1);
    }

    auto RegisterHooks() -> void
    {
        using namespace mhooks;

        MHookReGameInternalCommand(DELEGATE_ARG<InternalCommand>);

        MHookReGamePlayerSpawn(DELEGATE_ARG<Player_Spawn>);
        MHookReGamePlayerPreThink(DELEGATE_ARG<Player_PreThink>);
        MHookReGamePlayerAddPlayerItem(DELEGATE_ARG<Player_AddPlayerItem>);
        MHookReGamePlayerRemovePlayerItem(DELEGATE_ARG<Player_RemovePlayerItem>);
        MHookReGamePlayerSpawnEquip(DELEGATE_ARG<Player_SpawnEquip>);
        MHookReGamePlayerGiveDefaultItems(DELEGATE_ARG<Player_GiveDefaultItems>);
        MHookReGamePlayerJump(DELEGATE_ARG<Player_Jump>);
        MHookReGamePlayerResetMaxSpeed(DELEGATE_ARG<Player_ResetMaxSpeed>);

        MHookReGamePlayerPain(DELEGATE_ARG<Player_Pain>);
        MHookReGamePlayerDeathSound(DELEGATE_ARG<Player_DeathSound>);
        MHookReGamePlayerJoiningThink(DELEGATE_ARG<Player_JoiningThink>);
    }

    /*
    auto Radio(
        IReGameHook_CBasePlayer_Radio* chain,
        CBasePlayer* basePlayer,
        const char* message,
        const char* messageVerbose,
        short pitch,
        bool showIcon
    ) -> void {
        // No op
    }

    auto ImpulseCommands(const ReGamePlayerImpulseCommandsMChain& chain, PlayerBase* base) -> void
    {
        auto& player = g_players[basePlayer];
        if (!player.getImpulse()) {
            chain->callNext(basePlayer);
            return;
        }
        chain->callNext(basePlayer);
    }

    auto playerSetClientUserInfoModel(IReGameHook_CBasePlayer_SetClientUserInfoModel* chain, CBasePlayer* player,
      const char* infoBuffer, const char* newModel) -> void
    {
        chain->callNext(player, infoBuffer, "");
    }

    */

    /*auto chooseAppearance(IReGameHook_HandleMenu_ChooseAppearance* chain, CBasePlayer* basePlayer, int slot) -> void
    {
        auto& player = g_players[basePlayer];
        SelectAppearance(player, 0);
    }
    */

    auto Player::ShowAppearance() -> void
    {
        // getPreview()->CreatePreview();
        setMenu(MenuName::ChooseAppearance);
        amxxPlayer.ChooseDefaultSubclass(*this);
    }

    auto Player::SelectAppearance() -> void
    {
        // getPreview()->RemovePreview();
        setMenu(MenuName::Off);
        setTeam(Team::Human);
        setJoiningState(JoinState::GetIntoGame);
        TeamChangeUpdate();
        gameRules->CheckWinConditions();
    }

    auto Player::SelectItem(const char* name) -> bool
    {
        if (name == nullptr) {
            return false;
        }
        const auto item = FindItemInInventoryByHud(name);
        if (item == nullptr) {
            return false;
        }
        const auto activeItem = getActiveItem();
        if (activeItem != nullptr && !activeItem->CanHolster()) {
            return true;
        }
        if (item == activeItem) {
            return true;
        }
        if (!item->CanDeploy()) {
            return true;
        }
        if (activeItem != nullptr) {
            activeItem->Holster();
        }
        setLastItem(activeItem);
        setActiveItem(item);
        item->Deploy();
        ResetMaxSpeed();
        return true;
    }

    auto Player::FindItemInInventoryByHud(const char* name) const -> PlayerItemBase*
    {
        for (auto slot = 0; slot < MAX_ITEM_TYPES; ++slot) {
            auto item = getPlayerItems(slot);
            while (item != nullptr) {
                const auto weaponRef = weaponModule[item->vars->impulse];
                if (weaponRef) {
                    const auto& weapon = weaponRef->get();
                    if (str::Equals(weapon.getHud(), name)) {
                        return item;
                    }
                }
                item = item->next;
            }
        }
        return nullptr;
    }

    auto Player::ChangeClass(int classIndex, Player* attackerUnsafe, bool preSpawn) -> bool
    {
        auto attackerIndex = 0;
        if (attackerUnsafe != nullptr) {
            attackerIndex = attackerUnsafe->id();
        }
        ResetVars();
        if (amxxPlayerClass.ChangeClassPre(*this, classIndex, attackerIndex) >= ForwardReturn::Supercede) {
            return false;
        }
        const auto playerClassRef = playerClassModule[classIndex];
        if (!playerClassRef) {
            return false;
        }
        auto& playerClass = playerClassRef->get();
        setClass(classIndex);
        setSubclass(0);
        setTeam(playerClass.getTeam()); // put into pre/ost
        setSound(playerClass.getSound());
        setMelee(playerClass.getMelee());
        setNightVision(playerClass.getNightVision());
        if (preSpawn) {
            setProps(playerClass.getProps());
            ChangeModel(playerClass.getModel());
        } else {
            if (attackerUnsafe != nullptr) {
                auto& attacker = *attackerUnsafe;
                setDeaths(getDeaths() + 1);
                attacker.setFrags(attacker.getFrags() + 1);
                sendDeathMsg(TO_ALL, attacker.id(), id(), false, "teammate");
                sendScoreAttrib(TO_ALL, id(), 0);
                sendTeamInfo(TO_ALL, id(), getTeam());
                sendScoreInfo(TO_ALL, id(), getFrags(), getDeaths(), 0, getTeamId());
                sendScoreInfo(
                  TO_ALL, attacker.id(), attacker.getFrags(), attacker.getDeaths(), 0, attacker.getTeamId()
                );
            } else {
                sendTeamInfo(TO_ALL, id(), getTeam());
                sendDeathMsg(TO_ALL, id(), id(), false, "teammate");
                sendScoreAttrib(TO_ALL, id(), 0);
            }
            GiveDefaultItems();
            ChangeProps(playerClass.getProps());
            ChangeModel(playerClass.getModel());
        }
        gameRules->CheckWinConditions();
        amxxPlayerClass.ChangeClassPost(*this, classIndex, attackerIndex);
        return true;
    }

    auto Player::ChangeProps(int propsIndex, bool spawn) -> bool
    {
        const auto propsRef = playerPropsModule[propsIndex];
        if (!propsRef) {
            return false;
        }
        setProps(propsIndex);
        const auto& props = propsRef->get();
        if (props.getBaseHealth()) {
            // maybe all?
            GiveHealth(props.getBaseHealth() * getPlayersCount(Team::Unassigned, PlayersCountType::Alive));
        } else {
            GiveHealth(props.getHealth());
        }
        setGravity(props.getGravity());
        setBloodColor(props.getBloodColor());
        setRenderMode(props.getRenderMode());
        setRenderAmount(props.getRenderAmount());
        setRenderColor(props.getRenderColor());
        setRenderFx(props.getRenderFx());
        SetFootSteps(props.getFootSteps());
        if (spawn) {
            GiveArmor(std::max(props.getArmor(), getArmor()), std::max(props.getHelmet(), getHelmet()));
        } else {
            GiveArmor(props.getArmor(), props.getHelmet());
        }
        return true;
    }

    auto Player::ChangeModel(int modelIndex) -> bool
    {
        const auto modelRef = playerModelModule[modelIndex];
        if (!modelRef) {
            return false;
        }
        setModel(modelIndex);
        const auto& model = modelRef->get();
        const auto& modelOnce = model.getRandom();
        GiveModel(modelOnce);
        return true;
    }

    auto Player::GiveHealth(int health) -> void
    {
        setHealth(health);
        setMaxHealth(health);
    }

    auto Player::GiveArmor(int armor, bool helmet) -> void
    {
        setArmor(armor);
        if (getArmor() > 0) {
            setKevlar(helmet ? ArmorType::VestHelm : ArmorType::Kevlar);
        } else {
            setKevlar(ArmorType::None);
        }
        sendArmorType(*this, helmet);
    }

    auto Player::GiveModel(const PlayerModelHeader& model) -> void
    {
        cstrike_->SetPlayerModelEx(model.getPath().c_str());
        cstrike_->SetPlayerModel(false);
        base_->model_index_player = model.getModelIndex();
        vars_->model = AllocString(model.getPath().c_str()); // really need? maybe ""
        vars_->model_index = model.getModelIndex();
        setBody(model.getBody());
    }

    auto Player::SetFootSteps(bool footSteps) -> void
    {
        if (footSteps) {
            vars_->time_step_sound = 400;
            base_->time_step_sound = 0;
        } else {
            vars_->time_step_sound = 999;
            base_->time_step_sound = 999;
        }
    }

    auto Player::ResetFovZoom() -> void
    {
        vars_->field_of_view = DEFAULT_FOV;
        base_->fov = DEFAULT_FOV;
        base_->last_zoom = DEFAULT_FOV;
        base_->resume_zoom = false;
    }

    auto Player::isFrozen() const -> bool
    {
        return freezeEndTime_ != 0.0f;
    }

    auto Player::getFreezeEndTime() const -> float
    {
        return freezeEndTime_;
    }

    auto Player::Freeze(float freezeTime) -> void
    {
        freezeEndTime_ = g_global_vars->time + freezeTime;
        ResetMaxSpeed();
    }

    auto Player::RemoveFreeze() -> void
    {
        freezeEndTime_ = 0.0;
        ResetMaxSpeed();
    }
}
