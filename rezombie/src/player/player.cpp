#include "rezombie/player/player.h"
#include "rezombie/player/api/player.h"
#include "rezombie/player/api/player_class.h"
#include "rezombie/player/api/player_subclass.h"
#include "rezombie/player/modules/player_class.h"
#include "rezombie/player/modules/player_subclass.h"
#include "rezombie/player/modules/player_sounds.h"
#include "rezombie/models/modules/models.h"
#include "rezombie/models/modules/models_pack.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/players.h"
#include "rezombie/main/util.h"
#include "rezombie/messages/user_message.h"
#include <metamod/engine.h>
#include <metamod/utils.h>
#include <mhooks/reapi.h>

namespace rz
{
    using namespace core;
    using namespace metamod;

    /*
        void EXT_FUNC CBasePlayer::__API_HOOK(Killed)(entvars_t *pevAttacker, int iGib)
        {
            m_canSwitchObserverModes = false;

            if (m_LastHitGroup == HITGROUP_HEAD)
                m_bHeadshotKilled = true;

            CBaseEntity *pAttackerEntity = CBaseEntity::Instance(pevAttacker);

            if (!m_bKilledByBomb)
            {
                g_pGameRules->PlayerKilled(this, pevAttacker, g_pevLastInflictor);
            }

            MESSAGE_BEGIN(MSG_ONE, gmsgNVGToggle, nullptr, pev);
            WRITE_BYTE(0);
            MESSAGE_END();

            m_bNightVisionOn = false;

            for (int i = 1; i <= gpGlobals->maxClients; i++)
            {
                CBasePlayer *pObserver = UTIL_PlayerByIndex(i);

                if (!pObserver)
                    continue;

                if (pObserver->IsObservingPlayer(this))
                {
                    MESSAGE_BEGIN(MSG_ONE, gmsgNVGToggle, nullptr, pObserver->pev);
                    WRITE_BYTE(0);
                    MESSAGE_END();

                    pObserver->m_bNightVisionOn = false;
                }

                if (pObserver->m_hObserverTarget == this)
                    pObserver->m_flNextFollowTime = 0.0f;
            }

            if (m_pTank)
            {
                m_pTank->Use(this, this, USE_OFF, 0);
                m_pTank = nullptr;
            }

            SetAnimation(PLAYER_DIE);

            if (m_pActiveItem && m_pActiveItem->m_pPlayer)
            {
                switch (m_pActiveItem->m_iId)
                {
                    case WEAPON_HEGRENADE:
                    {
                        CHEGrenade *pHEGrenade = static_cast<CHEGrenade *>(m_pActiveItem);
                        if ((pev->button & IN_ATTACK) && m_rgAmmo[pHEGrenade->m_iPrimaryAmmoType])
                        {
                            ThrowGrenade(pHEGrenade, (pev->origin + pev->view_ofs), pev->angles, 1.5, pHEGrenade->m_usCreateExplosion);
                            m_rgAmmo[m_pActiveItem->PrimaryAmmoIndex()]--;
                            pHEGrenade->m_flStartThrow = 0;
                        }
                        break;
                    }
                    case WEAPON_FLASHBANG:
                    {
                        CFlashbang *pFlashbang = static_cast<CFlashbang *>(m_pActiveItem);
                        if ((pev->button & IN_ATTACK) && m_rgAmmo[pFlashbang->m_iPrimaryAmmoType])
                        {
                            ThrowGrenade(pFlashbang, (pev->origin + pev->view_ofs), pev->angles, 1.5);
                            m_rgAmmo[m_pActiveItem->PrimaryAmmoIndex()]--;
                            pFlashbang->m_flStartThrow = 0;
                        }
                        break;
                    }
                    case WEAPON_SMOKEGRENADE:
                    {
                        CSmokeGrenade *pSmoke = static_cast<CSmokeGrenade *>(m_pActiveItem);
                        if ((pev->button & IN_ATTACK) && m_rgAmmo[pSmoke->m_iPrimaryAmmoType])
                        {
                            ThrowGrenade(pSmoke, (pev->origin + pev->view_ofs), pev->angles, 1.5, pSmoke->m_usCreateSmoke);
                            m_rgAmmo[m_pActiveItem->PrimaryAmmoIndex()]--;
                            pSmoke->m_flStartThrow = 0;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            pev->modelindex = m_modelIndexPlayer;
            pev->deadflag = DEAD_DYING;
            pev->movetype = MOVETYPE_TOSS;
            pev->takedamage = DAMAGE_NO;

            pev->gamestate = HITGROUP_SHIELD_DISABLED;
            m_bShieldDrawn = false;

            pev->flags &= ~FL_ONGROUND;

    #ifdef REGAMEDLL_FIXES
            // FlashlightTurnOff()
            pev->effects &= ~EF_DIMLIGHT;
    #endif

    #ifndef REGAMEDLL_ADD
            if (fadetoblack.value == 0.0)
        {
            pev->iuser1 = OBS_CHASE_FREE;
            pev->iuser2 = ENTINDEX(edict());
            pev->iuser3 = ENTINDEX(ENT(pevAttacker));

            m_hObserverTarget = UTIL_PlayerByIndexSafe(pev->iuser3);

            MESSAGE_BEGIN(MSG_ONE, gmsgADStop, nullptr, pev);
            MESSAGE_END();
        }
        else
        {
            UTIL_ScreenFade(this, Vector(0, 0, 0), 3, 3, 255, (FFADE_OUT | FFADE_STAYOUT));
        }
    #else

            float flDyingDuration = GetSequenceDuration() + CGameRules::GetDyingTime();
            switch ((int)fadetoblack.value)
            {
                default:
                {
                    pev->iuser1 = OBS_CHASE_FREE;
                    pev->iuser2 = ENTINDEX(edict());
                    pev->iuser3 = ENTINDEX(ENT(pevAttacker));

                    m_hObserverTarget = UTIL_PlayerByIndexSafe(pev->iuser3);

                    MESSAGE_BEGIN(MSG_ONE, gmsgADStop, nullptr, pev);
                    MESSAGE_END();

                    break;
                }
                case FADETOBLACK_STAY:
                {
                    UTIL_ScreenFade(this, Vector(0, 0, 0), 0.8f, flDyingDuration, 255, (FFADE_OUT | FFADE_STAYOUT));
                    break;
                }
                case FADETOBLACK_AT_DYING:
                {
                    pev->iuser1 = OBS_CHASE_FREE;
                    pev->iuser2 = ENTINDEX(edict());
                    pev->iuser3 = ENTINDEX(ENT(pevAttacker));

                    m_hObserverTarget = UTIL_PlayerByIndexSafe(pev->iuser3);

                    MESSAGE_BEGIN(MSG_ONE, gmsgADStop, nullptr, pev);
                    MESSAGE_END();

                    UTIL_ScreenFade(this, Vector(0, 0, 0), 0.8f, flDyingDuration, 255, (FFADE_OUT));

                    break;
                }
            }
    #endif // REGAMEDLL_ADD

            SetScoreboardAttributes();

            if (m_iThrowDirection)
            {
                switch (m_iThrowDirection)
                {
                    case THROW_FORWARD:
                    {
                        UTIL_MakeVectors(pev->angles);
                        pev->velocity = gpGlobals->v_forward * RANDOM_FLOAT(100, 200);
                        pev->velocity.z = RANDOM_FLOAT(50, 100);
                        break;
                    }
                    case THROW_BACKWARD:
                    {
                        UTIL_MakeVectors(pev->angles);
                        pev->velocity = gpGlobals->v_forward * RANDOM_FLOAT(-100, -200);
                        pev->velocity.z = RANDOM_FLOAT(50, 100);
                        break;
                    }
                    case THROW_HITVEL:
                    {
                        if (FClassnameIs(pevAttacker, "player"))
                        {
                            UTIL_MakeVectors(pevAttacker->angles);

                            pev->velocity = gpGlobals->v_forward * RANDOM_FLOAT(200, 300);
                            pev->velocity.z = RANDOM_FLOAT(200, 300);
                        }
                        break;
                    }
                    case THROW_BOMB:
                    {
                        // TODO: fix test demo
                        pev->velocity = m_vBlastVector * (1.0f / m_vBlastVector.Length()) * float(2300.0f - m_vBlastVector.Length()) * 0.25f;
                        pev->velocity.z = (2300.0f - m_vBlastVector.Length()) / 2.75f;
                        break;
                    }
                    case THROW_GRENADE:
                    {
                        pev->velocity = m_vBlastVector * (1 / m_vBlastVector.Length()) * (500 - m_vBlastVector.Length());
                        pev->velocity.z = (350 - m_vBlastVector.Length()) * 1.5;
                        break;
                    }
                    case THROW_HITVEL_MINUS_AIRVEL:
                    {
                        if (FClassnameIs(pevAttacker, "player"))
                        {
                            UTIL_MakeVectors(pevAttacker->angles);
                            pev->velocity = gpGlobals->v_forward * RANDOM_FLOAT(200, 300);
                        }
                        break;
                    }
                    default:
                        break;
                }

                pev->angles.y = UTIL_VecToAngles(-pev->velocity).y;
                pev->v_angle.y = pev->angles.y;

                m_iThrowDirection = THROW_NONE;
            }

            m_iClientHealth = 0;
            MESSAGE_BEGIN(MSG_ONE, gmsgHealth, nullptr, pev);
            WRITE_BYTE(m_iClientHealth);
            MESSAGE_END();

            MESSAGE_BEGIN(MSG_ONE, gmsgCurWeapon, nullptr, pev);
            WRITE_BYTE(0);
            WRITE_BYTE(0xFF);
            WRITE_BYTE(0xFF);
            MESSAGE_END();

            SendFOV(0);

            CSGameRules()->CheckWinConditions();
            m_bNotKilled = false;

            BuyZoneIcon_Clear(this);

    #ifdef REGAMEDLL_ADD
            CSPlayer()->OnKilled();
    #endif

            SetThink(&CBasePlayer::PlayerDeathThink);
            pev->nextthink = gpGlobals->time + 0.1f;
            pev->solid = SOLID_NOT;

            if ((pev->health < -9000 && iGib != GIB_NEVER) || iGib == GIB_ALWAYS)
            {

    #ifndef REGAMEDLL_FIXES
                pev->solid = SOLID_NOT;
    #endif
                GibMonster();
                pev->effects |= EF_NODRAW;

    #ifndef REGAMEDLL_FIXES
                CSGameRules()->CheckWinConditions();
    #endif
                return;
            }

            DeathSound();

            pev->angles.x = 0;
            pev->angles.z = 0;
        }
    */
    auto Player::init(PlayerBase* base) -> void {
        edict_ = base->GetEdict();
        vars_ = &edict_->vars;
        base_ = base;
        cstrike_ = base->GetCsPlayer();
        ResetVars(true);
    }

    auto Player::disconnect() -> void {
        edict_ = nullptr;
        vars_ = nullptr;
        base_ = nullptr;
        cstrike_ = nullptr;
    }

    auto Player_Spawn(ReHookPlayerSpawn* chain, PlayerBase* base) -> void {
        auto& player = Players[base];
        if (player.isJustConnected()) {
            chain->CallNext(base);
            player.setHideHud(player.getHideHud() | HIDE_HUD_MONEY);
            return;
        }
        if (!player.isPlayableTeam()) {
            chain->CallNext(base);
            return;
        }
        if (GameRules.isCanMove()) {
            player.setIUser3(player.getIUser3() & ~NO_MOVE_PLAYER_FLAGS);
        }
        const auto newClass = GameRules.defaultPlayerClass(GameRules.getRespawnTeam());
        if (player.getClass() != newClass) {
            player.setKilled(true);
            player.setClass(newClass);
        }
        player.ChangeClass(player.getClass(), &player, true);
        // check body for alive
        const auto body = player.getBody();
        chain->CallNext(base);
        player.setBody(body);
        //player.setThirdCamera(true);
    }

    auto Player_TakeDamage(
        ReHookPlayerTakeDamage* chain,
        PlayerBase* base,
        EntityVars* inflictorVars,
        EntityVars* attackerVars,
        float& damage,
        int damageType
    ) -> qboolean {
        const auto& weaponRef = Weapons[inflictorVars->impulse];
        if (weaponRef) {
            const auto& weapon = weaponRef->get();
            if (weapon.getAlwaysDamage() != -1) {
                damage = static_cast<float>(weapon.getAlwaysDamage());
            }
        }
        return chain->CallNext(base, inflictorVars, attackerVars, damage, damageType);
    }

    auto Player_Killed(ReHookPlayerKilled* chain, PlayerBase* base, EntityVars* attackerVars, int gib) -> void {
        if (attackerVars && attackerVars != base->vars) {
            const auto& attackerBase = EntityPrivateData<PlayerBase>(attackerVars->containing_entity);
            if (attackerBase != nullptr) {
                const auto& attacker = Players[attackerBase];
                const auto& activeItem = attacker.getActiveItem(); // to inflictor
                if (activeItem != nullptr) {
                    const auto& weaponRef = Weapons[activeItem->vars->impulse];
                    if (weaponRef) {
                        const auto& weapon = weaponRef->get();
                        if (weapon.getGibs()) {
                            gib = GIB_ALWAYS;
                        }
                    }
                }
            }
        }
        chain->CallNext(base, attackerVars, gib);
        auto& player = Players[base];
        player.setIUser1(ObserverMode::None);
        //player.setThirdCamera(true);
    }

    auto Player_PreThink(ReHookPlayerPreThink* chain, PlayerBase* base) {
        chain->CallNext(base);
        auto& player = Players[base];
        player.PreviewUpdate();
        player.MapCameraUpdate();
        if (!player.isAlive()) {
            return;
        }
        player.WorldPreviewUpdate();
        if (player.isFrozen() && player.getFreezeEndTime() <= g_global_vars->time) {
            player.RemoveFreeze();
        }
        player.LongJumpCooldown();
        //player.ThirdCameraUpdate();
    }

    auto Player_PostThink(ReHookPlayerPreThink* chain, PlayerBase* base) {
        auto& player = Players[base];
        if (player.isAlive()) {
            if (player.getImpulse() == 100) {
                player.SwitchFlashlight(!player.Flashlight().isEnabled());
                player.setImpulse(0);
            }
        }
        chain->CallNext(base);
        player.ThirdCameraUpdate();
        if (!player.isAlive()) {
            return;
        }
        player.JumpThink();
    }

    auto Player_AddPlayerItem(ReHookPlayerAddPlayerItem*, PlayerBase* base, PlayerItemBase* item) -> qboolean {
        auto& player = Players[base];
        const auto itemSlot = item->GetCsPlayerItem()->item_info.slot;
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
        if (GameRules.ShouldSwitchWeapon(player, item)) {
            player.SwitchWeapon(item);
        }
        player.setHideHud(player.getHideHud() & ~HIDE_HUD_WEAPONS);
        return true;
    }

    auto Player_RemovePlayerItem(ReHookPlayerRemovePlayerItem*, PlayerBase* base, PlayerItemBase* item) -> qboolean {
        auto& player = Players[base];
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
        auto previous = player.getPlayerItems(itemSlot);
        if (previous == item) {
            player.setPlayerItems(itemSlot, item->next);
            return true;
        }
        while (previous != nullptr && previous->next != item) {
            previous = previous->next;
        }
        if (previous != nullptr) {
            previous->next = item->next;
            return true;
        }
        return false;
    }

    auto Player_DropPlayerItem(ReHookPlayerDropPlayerItem*, PlayerBase* base, const char*) -> EntityBase* {
        auto& player = Players[base];
        return player.DropPlayerItem(player.getActiveItem());
    }

    auto Player_SpawnEquip(ReHookPlayerOnSpawnEquip*, PlayerBase* base, bool, bool) -> void {
        auto& player = Players[base];
        player.ChangeProps(player.getProps(), true);
        /*if (!player.isKilled()) {
            return;
        }*/
        player.GiveDefaultItems();
    }

    auto Player_GiveDefaultItems(ReHookPlayerGiveDefaultItems*, PlayerBase* base) -> void {
        auto& player = Players[base];
        player.RemoveAllItems();
        player.GiveWeapon(player.getMelee(), GiveType::Replace);
        const auto& classRef = Classes[player.getClass()];
        if (classRef) {
            const auto& playerClass = classRef->get();
            playerClass.executeGiveDefaultItems(player);
        }
        PlayerApi.GiveDefaultItems(player, player.getClass());
    }

    auto Player_Jump(ReHookPlayerJump* chain, PlayerBase* base) -> void {
        //chain->CallNext(base);
        auto& player = Players[base];
        if (player.getIUser3() & NO_MOVE_PLAYER_FLAGS) {
            return;
        }
        player.Jump();
        player.LongJump();
    }

    auto Player_UpdateClientData(ReHookPlayerUpdateClientData* chain, PlayerBase* base) -> void {
        auto& player = Players[base];
        player.FlashlightUpdate();
        chain->CallNext(base);
    }

    auto Player_ImpulseCommands(ReHookPlayerImpulseCommands* chain, PlayerBase* base) -> void {
        auto& player = Players[base];
        chain->CallNext(base);
    }

    auto Player_AddAccount(ReHookPlayerAddAccount*, PlayerBase*, int, RewardType, bool) -> void {
        // no op
    }

    auto Player_Radio(
        ReHookPlayerRadio* chain,
        PlayerBase* base,
        const char* message,
        const char* messageVerbose,
        short pitch,
        bool showIcon
    ) -> void {
        // No op
    }

    auto Player_ResetMaxSpeed(ReHookPlayerResetMaxSpeed*, PlayerBase* base) -> void {
        auto& player = Players[base];
        // if joining
        if (player.getObserverMode() != ObserverMode::None) {
            player.setMaxSpeed(900);
            return;
        }
        if (!GameRules.isCanMove() || player.isFrozen()) {
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

    auto Player_Pain(ReHookPlayerPain* chain, PlayerBase* base, HitBoxGroup lastHitGroup, bool hasArmour) -> void {
        const auto& player = Players[base];
        const auto soundsRef = PlayerSounds[player.getSounds()];
        if (!soundsRef) {
            chain->CallNext(base, lastHitGroup, hasArmour);
            return;
        }
        auto& sound = soundsRef->get();
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

    auto Player_DeathSound(ReHookPlayerDeathSound* chain, PlayerBase* base) -> void {
        const auto& player = Players[base];
        const auto soundsRef = PlayerSounds[player.getSounds()];
        if (!soundsRef) {
            chain->CallNext(base);
            return;
        }
        auto& sound = soundsRef->get();
        UTIL_EmitSound(player, SoundChannel::Voice, sound.getRandom(PlayerSoundType::Death));
    }

    auto Player_JoiningThink(ReHookPlayerJoiningThink* chain, PlayerBase* base) -> void {
        chain->CallNext(base);
        auto& player = Players[base];

        switch (player.getJoiningState()) {
            case JoinState::Joined: {
                break;
            }
            case JoinState::ShowText: {
                break;
            }
            case JoinState::ReadingText: {
                break;
            }
            case JoinState::ShowTeamSelect: {
                break;
            }
            case JoinState::PickingTeam: {
                break;
            }
            case JoinState::GetIntoGame: {
                break;
            }
        }

        if (player.getJoiningState() == JoinState::Joined || player.getJoiningState() == JoinState::GetIntoGame) {
            return;
        }
        /*if (player.getJoiningState() != JoinState::ShowTeamSelect) {
            chain.CallNext(base);
            return;
        }*/
    }

    auto Player_DeathThink(ReHookPlayerDeathThink* chain, PlayerBase* base) -> void {
        auto& player = Players[base];
        if (player.getJoiningState() != JoinState::Joined) {
            return;
        }
        if (player.getFlags() & FL_ON_GROUND) {
            const float forward = player.getVelocity().Length() - 20;
            if (forward <= 0) {
                player.setVelocity(VECTOR_ZERO);
            } else {
                player.setVelocity(forward * player.getVelocity().Normalize());
            }
        }

        if (player.hasWeapons()) {
            player.forEachItem([&player](PlayerItem* item) {
                if (item->CanDrop()) {
                    player.DropPlayerItem(item);
                    item->vars->velocity.x = RandomFloat(24.0, 64.0) * (RandomLong(0, 1) ? 1.f : -1.f);
                    item->vars->velocity.y = RandomFloat(24.0, 64.0) * (RandomLong(0, 1) ? 1.f : -1.f);
                    item->vars->velocity.z = RandomFloat(64.0, 128.0);
                }
                return false;
            });
            player.RemoveAllItems(true);
            player.setDeadTime(g_global_vars->time + 2);
            sendBarTime(player, 2);
        }

        if (player.getMoveType() != MoveTypeEntity::None && (player.getFlags() & FL_ON_GROUND)) {
            player.setMoveType(MoveTypeEntity::None);
        }

        auto angles = player.getAngles();
        angles.x = 0;
        player.setAngles(angles);

        if (player.getDeadFlag() == DeathState::Dying) {
            if (base->vars->model_index && !base->sequence_finished) {
                auto interval = g_global_vars->time - base->vars->anim_time;
                if (interval <= 0.001f) {
                    base->vars->anim_time = g_global_vars->time;
                } else {
                    if (base->vars->anim_time == 0.f) {
                        interval = 0.f;
                    }
                    base->vars->frame += interval * base->frame_rate;
                    base->vars->anim_time = g_global_vars->time;
                    if (base->vars->frame >= 255.f) {
                        base->vars->frame = 255.f;
                        base->sequence_finished = true;
                    }
                }
            } else {
                player.setDeadFlag(DeathState::Dead);
                player.setEffects(player.getEffects() | EF_NO_INTERPOLATE);
            }
        }
        if (player.getDeadTime() <= g_global_vars->time) {
            if (GameRules.PlayerCanRespawn(player)) {
                player.RoundRespawn();
            } else {
                //if ({// && !(physics_flags & PFLAG_OBSERVER)) {
                base->GetCsPlayer()->StartDeathCam();
                //}
            }
        }
    }

    auto InternalCommand(ReHookInternalCommand* chain, Edict* client, const char* command, const char* arg1) -> void {
        auto& player = Players[client];
        if (str::Equals(command, "chooseteam")) {
            if (player.getJoiningState() == JoinState::Joined) {
                chain->CallNext(client, command, arg1);
            }
            return;
        }
        if (str::Equals(command, "nightvision")) {
            if (!player.isAlive() || !player.NightVision().getId()) {
                return;
            }
            if (g_global_vars->time < player.getLastCommandTime(TrackCommands::NightVision)) {
                return;
            }
            player.setLastCommandTime(TrackCommands::NightVision, g_global_vars->time + 0.3f);
            player.SwitchNightVision(!player.NightVision().isEnabled());
            return;
        }
        if (player.SelectItem(command)) {
            return;
        }
        chain->CallNext(client, command, arg1);
    }

    auto RegisterPlayerHooks() -> void {
        auto* hooks = regamedll_api::HookChains();

        hooks->InternalCommand()->RegisterHook(&InternalCommand);

        hooks->PlayerSpawn()->RegisterHook(&Player_Spawn);
        hooks->PlayerTakeDamage()->RegisterHook(&Player_TakeDamage);
        hooks->PlayerKilled()->RegisterHook(&Player_Killed);
        hooks->PlayerPreThink()->RegisterHook(&Player_PreThink);
        hooks->PlayerPostThink()->RegisterHook(&Player_PostThink);
        hooks->PlayerAddPlayerItem()->RegisterHook(&Player_AddPlayerItem);
        hooks->PlayerRemovePlayerItem()->RegisterHook(&Player_RemovePlayerItem);
        hooks->PlayerDropPlayerItem()->RegisterHook(&Player_DropPlayerItem);
        hooks->PlayerOnSpawnEquip()->RegisterHook(&Player_SpawnEquip);
        hooks->PlayerGiveDefaultItems()->RegisterHook(&Player_GiveDefaultItems);
        hooks->PlayerResetMaxSpeed()->RegisterHook(&Player_ResetMaxSpeed);
        hooks->PlayerJump()->RegisterHook(&Player_Jump);

        hooks->PlayerUpdateClientData()->RegisterHook(&Player_UpdateClientData);
        hooks->PlayerImpulseCommands()->RegisterHook(&Player_ImpulseCommands);
        hooks->PlayerAddAccount()->RegisterHook(&Player_AddAccount);
        hooks->PlayerRadio()->RegisterHook(&Player_Radio);

        hooks->PlayerPain()->RegisterHook(&Player_Pain);
        hooks->PlayerDeathSound()->RegisterHook(&Player_DeathSound);
        hooks->PlayerJoiningThink()->RegisterHook(&Player_JoiningThink);
        hooks->PlayerDeathThink()->RegisterHook(&Player_DeathThink);
    }

    /*auto chooseAppearance(IReGameHook_HandleMenu_ChooseAppearance* chain, CBasePlayer* basePlayer, int slot) -> void
    {
        auto& player = g_players[basePlayer];
        Joined(player, 0);
    }
    */

    auto Player::Joining() -> void {
        PlayerApi.Joining(*this);
        //setMenu(MenuName::ChooseAppearance);
    }

    auto Player::Joined() -> void {
        //setMenu(MenuName::Off);
        setTeam(Team::Human);
        setJoiningState(JoinState::GetIntoGame);
        TeamChangeUpdate();
        PlayerApi.Joined(*this);
    }

    auto Player::SelectItem(const char* name) -> bool {
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

    auto Player::FindItemInInventoryByHud(const char* name) const -> PlayerItemBase* {
        for (auto slot = 0; slot < MAX_ITEM_TYPES; ++slot) {
            auto item = getPlayerItems(slot);
            while (item != nullptr) {
                const auto weaponRef = Weapons[item->vars->impulse];
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

    auto Player::ChangeClass(int classId, Player* attackerUnsafe, bool preSpawn) -> ForwardReturn {
        ResetVars();
        const auto attackerId = attackerUnsafe ? attackerUnsafe->id() : 0;
        const auto result = PlayerClassApi.ChangeClassPre(*this, classId, attackerId);
        if (result > ForwardReturn::Continue) {
            return result;
        }
        const auto& classRef = Classes[classId];
        if (!classRef) {
            return result;
        }
        const auto& playerClass = classRef->get();
        setClass(classId);
        setTeam(playerClass.getTeam());
        setProps(playerClass.getProps());
        setSounds(playerClass.getSounds());
        setMelee(playerClass.getMelee());
        //setNightVision(playerClass.getNightVision());
        ChangeModel(playerClass.getModelsPack());
        ChangeSubclass(getKeepSubclass(classId));
        if (!preSpawn) {
            ChangeProps(getProps());
            GiveDefaultItems();
        }
        if (!attackerUnsafe) {
            sendTeamInfo(TO_ALL, id(), playerClass.getTeam());
        }
        PlayerClassApi.ChangeClassPost(*this, classId, attackerId);
        if (attackerUnsafe) {
            sendTeamInfo(TO_ALL, id(), playerClass.getTeam());
        }
        /*if (!preSpawn) {
            if (attackerUnsafe) {
                auto& attacker = *attackerUnsafe;
                sendDeathMsg(TO_ALL, attacker.id(), id(), INFECTION_ICON);
                sendScoreAttrib(TO_ALL, id(), 0);
                sendTeamInfo(TO_ALL, id(), playerClass.getTeam());
                setDeaths(getDeaths() + 1);
                sendScoreInfo(TO_ALL, id(), getFrags(), getDeaths(), playerClass.getTeam());
                attacker.setFrags(attacker.getFrags() + 1);
                sendScoreInfo(
                    TO_ALL,
                    attacker.id(),
                    attacker.getFrags(),
                    attacker.getDeaths(),
                    attacker.getTeam()
                );
            } else {
                sendTeamInfo(TO_ALL, id(), playerClass.getTeam());
                sendDeathMsg(TO_ALL, id(), id(), INFECTION_ICON);
                sendScoreAttrib(TO_ALL, id(), 0);
            }
        }*/
        return result;
    }

    auto Player::ChangeSubclass(int subclassId) -> ForwardReturn {
        const auto result = PlayerSubclassApi.ChangeSubclassPre(*this, subclassId);
        if (result > ForwardReturn::Continue) {
            return result;
        }
        const auto& subclassRef = Subclasses[subclassId];
        if (!subclassRef) {
            return result;
        }
        const auto& subclass = subclassRef->get();
        setSubclass(subclassId);
        setProps(subclass.getProps());
        setSounds(subclass.getSounds());
        setMelee(subclass.getMelee());
        //getNightVision().setId(0);
        //switchFlashlight
        ChangeModel(subclass.getModel());
        PlayerSubclassApi.ChangeSubclassPost(*this, subclassId);
        return result;
    }

    auto Player::ChangeProps(int propsId, bool isSpawn) -> bool {
        const auto propsRef = Props[propsId];
        if (!propsRef) {
            return false;
        }
        setProps(propsId);
        const auto& props = propsRef->get();
        if (props.getBaseHealth()) {
            GiveHealth(props.getBaseHealth() * getPlayersCount(PlayersCountFlags::Alive));
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
        if (isSpawn) {
            GiveArmor(std::max(props.getArmor(), getArmor()), std::max(props.getHelmet(), getHelmet()));
        } else {
            GiveArmor(props.getArmor(), props.getHelmet());
        }
        return true;
    }

    auto Player::ChangeModel(int modelId) -> bool {
        const auto& modelsPackRef = ModelsPack[modelId];
        if (modelsPackRef) {
            const auto& modelsPack = modelsPackRef->get();
            if (modelsPack.count()) {
                modelId = modelsPack.getRandom();
            } else {
                modelId = Models.getDefaultPlayerModel();
            }
        }
        const auto& modelRef = Models[modelId];
        if (!modelRef) {
            return false;
        }
        setModel(modelId);
        GiveModel(*modelRef);
        return true;
    }

    auto Player::GiveHealth(int health) -> void {
        setHealth(health);
        setMaxHealth(health);
    }

    auto Player::GiveArmor(int armor, bool helmet) -> void {
        setArmor(armor);
        if (getArmor() > 0) {
            setKevlar(helmet ? ArmorType::VestHelm : ArmorType::Kevlar);
        } else {
            setKevlar(ArmorType::None);
        }
        sendArmorType(*this, helmet);
    }

    auto Player::GiveModel(const Model& model) -> void {
        setModelIndex(model.getPrecacheId());
        setHitBoxes(model.getPrecacheId());
        setBody(model.getBody());
        setSkin(model.getSkin());
    }

    auto Player::SetFootSteps(bool footSteps) -> void {
        if (footSteps) {
            vars_->time_step_sound = 400;
            base_->time_step_sound = 0;
        } else {
            vars_->time_step_sound = 999;
            base_->time_step_sound = 999;
        }
    }

    auto Player::ResetFovZoom() -> void {
        vars_->field_of_view = DEFAULT_FOV;
        base_->fov = DEFAULT_FOV;
        base_->last_zoom = DEFAULT_FOV;
        base_->resume_zoom = false;
    }

    auto Player::isFrozen() const -> bool {
        return getFreezeEndTime() != 0.f;
    }

    auto Player::Freeze(float freezeTime) -> void {
        setFreezeEndTime(g_global_vars->time + freezeTime);
        ResetMaxSpeed();
    }

    auto Player::RemoveFreeze() -> void {
        setFreezeEndTime(0.f);
        ResetMaxSpeed();
    }
}
