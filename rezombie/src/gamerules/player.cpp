#include "rezombie/gamerules/game_rules.h"
#include "rezombie/messages/engine_message.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/player/players.h"
#include "rezombie/map/environment.h"
#include "rezombie/main/util.h"
#include <cssdk/public/regamedll/cs_player_item.h>
#include <metamod/engine.h>

#include "rezombie/modes/modules/modes.h"

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto TeamGameRules::ClientConnected(
        Edict* entity,
        const char* name,
        const char* address,
        char* rejectReason
    ) -> qboolean {
        return OriginalGameRules->CsGameRules()->ClientConnected(entity, name, address, rejectReason);
    }

    auto TeamGameRules::ClientCommand(PlayerBase* player, const char* command) -> qboolean {
        return OriginalGameRules->CsGameRules()->ClientCommand(player, command);
    }

    auto TeamGameRules::ClientCommandDeadOrAlive(PlayerBase* player, const char* command) -> qboolean {
        return OriginalGameRules->CsGameRules()->ClientCommandDeadOrAlive(player, command);
    }

    auto TeamGameRules::ClientUserInfoChanged(PlayerBase* base, char* infoBuffer) -> void {
        auto value = InfoKeyValue(infoBuffer, "_cl_autowepswitch");
        if (str::Equals(value, "")) {
            base->auto_wep_switch = 1;
        } else {
            base->auto_wep_switch = std::stoi(value);
        }
        value = InfoKeyValue(infoBuffer, "_ah");
        if (str::Equals(value, "")) {
            base->show_hints = true;
        } else {
            base->show_hints = std::stoi(value) != 0;
        }
    }

    auto TeamGameRules::ClientDisconnected(Edict* client) -> void {
        OriginalGameRules->CsGameRules()->ClientDisconnected(client);
        /* if (client == nullptr) {
            CheckWinConditions();
            return;
        }
        auto basePlayer = EntityPrivateData<PlayerBase>(client);
        if (basePlayer == nullptr) {
            CheckWinConditions();
            return;
        }
        auto& player = Players[basePlayer];
        player.setDisconnected(true);
        player.setDeadFlag(DeathState::Dead);
        // player->SetScoreboardAttributes();
        sendScoreInfo(TO_ALL, player.id(), 0, 0, 0, 0);
        sendTeamInfo(TO_ALL, player.id(), Team::Unassigned);

        //    // FireTargets("game_playerleave", pPlayer, pPlayer, USE_TOGGLE, 0);

        player.RemoveAllItems(true);

        //    if (player->observer) {
        //        // player->m_pObserver->SUB_Remove();
        //    }

        // PlayerBase* pObserver = nullptr;
        // while ((pObserver = UTIL_FindEntityByClassname(pObserver, "player"))) {
        //     if (FNullEnt(pObserver->edict())) {
        //         break;
        //     }
        //     if (!pObserver->pev || pObserver == pPlayer) {
        //         continue;
        //     }
        //     if (pObserver->m_hObserverTarget != pPlayer) {
        //         continue;
        //     }
        //     pObserver->pev->iuser1 = OBS_NONE;
        //     pObserver->m_flNextFollowTime = 0.0;
        //     pObserver->Observer_SetMode(pObserver->pev->iuser1);
        // }

        //    // player->MakeDormant();
        {
            basePlayer->vars->flags |= FL_DORMANT;
            basePlayer->vars->solid = SolidType::NotSolid;
            basePlayer->vars->move_type = MoveTypeEntity::None;
            basePlayer->vars->effects |= EF_NO_DRAW;
            basePlayer->vars->next_think = 0;
            SetOrigin(client, basePlayer->vars->origin);
        }
        CheckWinConditions();/*/
    }

    auto TeamGameRules::InitHud(PlayerBase* base) -> void {
        const auto& player = Players[base];
        netLightStyle(player, 0, Environment.getLight());
        sendFog(player, Environment.getFog());
        if (Environment.getWeatherType() != WeatherType::None) {
            sendReceiveW(player, Environment.getWeatherType());
        }
        UpdateGameMode(player);
        sendViewMode(player);
        sendScoreInfo(player, player, 0, 0, player.getTeam());
        sendShadowIdx(player, shadowSprite_);
        sendTeamScore(player, Team::Human, getTeamWins(Team::Human));
        sendTeamScore(player, Team::Zombie, getTeamWins(Team::Zombie));
        sendForceCam(player, mp_forcecamera->value != 0, mp_forcechasecam->value != 0, mp_fadetoblack->value != 0);
        Players.forEachConnected(
            [&player](const auto& target) {
                sendScoreInfo(player, target.id(), target.getFrags(), target.getDeaths(), target.getTeam());
            }
        );
        if (getGameState() != GameState::Over) {
            SendMotdToClient(player);
        }
        Players.forEachConnected(
            [&player](auto& target) {
                sendTeamInfo(player, target, target.getTeam());
                target.SetScoreboardAttributes(player);
                sendHealthInfo(player, target, target.getHealth());
                sendAccount(player, target, target.getAccount());
                if (player.id() != target.id() && target.isAlive()) {
                    sendRadar(player, target, target.getOrigin());
                }
            }
        );
    }

    auto TeamGameRules::UpdateGameMode(PlayerBase* base) -> void {
        const auto& player = Players[base];
        sendGameMode(player, 1);
    }

    auto TeamGameRules::PlayerFallDamage(PlayerBase* player) -> float {
        if (!mp_falldamage->value) {
            return 0;
        }
        player->fall_velocity -= MAX_PLAYER_SAFE_FALL_SPEED;
        return player->fall_velocity * DAMAGE_FOR_FALL_SPEED * 1.25f;
    }

    auto TeamGameRules::PlayerCanTakeDamage(PlayerBase* player, EntityBase* attacker) -> qboolean {
        if (getGameState() != GameState::Playing && getRoundState() != RoundState::Playing) {
            return false;
        }
        if (attacker == player) {
            return true;
        }
        if (GetPlayerRelationship(player, attacker) != PlayerRelationship::Teammate) {
            return true;
        }
        return false;
    }

    auto TeamGameRules::PlayerThink(PlayerBase* base) -> void {
        auto& player = Players[base];
        if (!isCanMove()) {
            player.setCanShoot(false);
            player.setButton(0);
            player.setButtonPressed(0);
            player.setButtonReleased(0);
            return;
        }
        player.setCanShoot(true);
        if (player.getWaterLevel() == 3) {
            const auto activeItem = player.getActiveItem();
            if (activeItem != nullptr && activeItem->IsWeapon() &&
                activeItem->GetCsPlayerItem()->item_info.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER) {
                player.setCanShoot(false);
            }
        }
        if (player.getJoiningState() == JoinState::ShowTeamSelect) {
            player.setJoiningState(JoinState::PickingTeam);
            // not by menu id
            if (player.getMenu() != MenuName::ChooseAppearance) {
                player.Joining();
            }
        }
    }

    auto TeamGameRules::PlayerSpawn(PlayerBase* base) -> void {
        auto& player = Players[base];
        if (player.isJustConnected()) {
            return;
        }
        const auto autoWeaponSwitch = base->auto_wep_switch;
        base->auto_wep_switch = 1;
        player.setWeapons(player.getWeapons() | (1 << WEAPON_SUIT));
        player.SpawnEquip();
        base->auto_wep_switch = autoWeaponSwitch;
    }

    auto TeamGameRules::PlayerCanRespawn(PlayerBase* base) -> qboolean {
        if (getGameState() == GameState::Over) {
            return false;
        }
        switch (getRoundState()) {
            case RoundState::Playing: {
                const auto modeRef = Modes[getMode()];
                if (modeRef) {
                    const auto& mode = modeRef->get();
                    if (mode.getRespawn() == RespawnType::Off) {
                        return false;
                    }
                }
                break;
            }
            case RoundState::Terminate: {
                return false;
            }
            default: {
                break;
            }
        }
        const auto& player = Players[base];
        if (!player.isPlayableTeam()) {
            return false;
        }
        // if (player.getMenu() == MenuName::ChooseAppearance) {
        //     return false;
        // }
        return true;
    }

    auto TeamGameRules::PlayerKilled(
        PlayerBase* victimBase,
        EntityVars* killerVars,
        EntityVars* inflictor
    ) -> void {
        auto& victim = Players[victimBase];
        DeathNotice(victimBase, killerVars, inflictor);
        victimBase->physics_flags &= ~PFLAG_ON_TRAIN;
        victimBase->train = (TRAIN_NEW | TRAIN_OFF);
        victim.setKilled(true);
        victim.setDeaths(victim.getDeaths() + 1);
        PlayerBase* killerPlayer = nullptr;
        const auto killerEntity = EntityPrivateData<EntityBase>(killerVars->containing_entity);
        if (killerEntity != nullptr && killerEntity->GetClassify() == Classify::Player) {
            killerPlayer = static_cast<PlayerBase*>(killerEntity);
        }
        // FireTargets("game_playerdie", pVictim, pVictim, USE_TOGGLE, 0);
        if (victimBase->vars == killerVars) {
            victim.setFrags(victim.getFrags() - 1);
        } else if (killerPlayer != nullptr && killerPlayer->IsPlayer()) {
            if (GetPlayerRelationship(victimBase, killerPlayer) != PlayerRelationship::Teammate) {
                auto& killer = Players[killerPlayer];
                killer.setFrags(killer.getFrags() + PointsForKill(killer, victim));
                // FireTargets("game_playerkill", killerPlayer, killerPlayer, USE_TOGGLE, 0);
            }
        } else {
            killerVars->frags -= 1;
        }
        sendScoreInfo(TO_ALL, victim.id(), victim.getFrags(), victim.getDeaths(), victim.getTeam());
        if (killerPlayer != nullptr && killerPlayer->IsPlayer()) {
            auto& killer = Players[killerEntity->EdictIndex()];
            sendScoreInfo(TO_ALL, killer.id(), killer.getFrags(), killer.getDeaths(), killer.getTeam());
            killer.setNextSprayTime(0.0);
        }
    }

    auto TeamGameRules::PointsForKill(PlayerBase*, PlayerBase*) -> int {
        return 1;
    }

    auto TeamGameRules::DeathNotice(
        PlayerBase* victimBase, EntityVars* killerVars, EntityVars* inflictorVars
    )
        -> void {
        const auto& victim = Players[victimBase];
        auto killerId = 0;
        std::string_view weaponName{"world"};
        if (killerVars->flags & FL_CLIENT) {
            const auto killerEdict = killerVars->containing_entity;
            killerId = IndexOfEdict(killerEdict);
            if (inflictorVars != nullptr) {
                if (inflictorVars == killerVars) {
                    const auto attacker = EntityPrivateData<PlayerBase>(killerEdict);
                    if (attacker != nullptr && attacker->IsPlayer()) {
                        const auto activeItem = attacker->active_item;
                        if (activeItem != nullptr) {
                            weaponName = activeItem->GetCsPlayerItem()->item_info.name;
                        }
                    }
                } else {
                    weaponName = AllocString(inflictorVars->class_name).CStr();
                }
            }
        } else if (inflictorVars != nullptr) {
            weaponName = AllocString(inflictorVars->class_name).CStr();
        }
        size_t cutWeaponPosition = weaponName.find('_');
        if (cutWeaponPosition && cutWeaponPosition <= weaponName.length()) {
            weaponName = weaponName.substr(cutWeaponPosition + 1);
        }
        sendDeathMsg(TO_ALL, killerId, victim.id(), weaponName, victim.isHeadshotKilled());
    }

    auto TeamGameRules::GetPlayerSpawnSpot(PlayerBase* base) -> Edict* {
        auto& player = Players[base];
        auto spawn = base->GetCsPlayer()->EntSelectSpawnPoint(); // to be removed
        player.setOrigin(spawn->vars.origin + Vector(0, 0, 1));
        player.setViewAngle(VECTOR_ZERO);
        player.setVelocity(VECTOR_ZERO);
        player.setAngles(spawn->vars.angles);
        player.setPunchAngle(VECTOR_ZERO);
        player.setFixAngleMode(FixAngleMode::ForceViewAngles);
        /*if (spawnSpot->v.target) {
            FireTargets(STRING(pentSpawnSpot->v.target), pPlayer, pPlayer, USE_TOGGLE, 0);
        }*/
        return spawn;
    }

    /*
    bool IsSpawnPointValid(CBaseEntity *pPlayer, CBaseEntity *pSpot)
    {
        CBaseEntity *pEntity = nullptr;
        while ((pEntity = UTIL_FindEntityInSphere(pEntity, pSpot->pev->origin, MAX_PLAYER_USE_RADIUS))) {
            if (pEntity->IsPlayer() && pEntity != pPlayer)
                return FALSE;
        }

        return true;
    }

    bool SelectSpawnSpot(const char *pEntClassName, CBaseEntity *&pSpot)
    {
        Edict *pPlayer = edict();

        pSpot = UTIL_FindEntityByClassname(pSpot, pEntClassName);

        if (FNullEnt(pSpot)) {
            pSpot = UTIL_FindEntityByClassname(pSpot, pEntClassName);
        }

        CBaseEntity *pFirstSpot = pSpot;

        do
        {
            if (pSpot)
            {
                if (IsSpawnPointValid(this, pSpot))
                {
                    if (pSpot->pev->origin == VECTOR_ZERO)
                    {
                        pSpot = UTIL_FindEntityByClassname(pSpot, pEntClassName);
                        continue;
                    }
                    return true;
                }
            }
            pSpot = UTIL_FindEntityByClassname(pSpot, pEntClassName);
        }
        while (pSpot != pFirstSpot);

        if (!FNullEnt(pSpot)) {
            return true;
        }

        return false;
    }

    CBaseEntity *g_pLastCTSpawn = nullptr;
    CBaseEntity *g_pLastTerroristSpawn = nullptr;

    Edict *EntSelectSpawnPoint(PlayerBase *player)
    {
        CBaseEntity *pSpot;

        if (m_iTeam == CT) {
            pSpot = g_pLastCTSpawn;
            SelectSpawnSpot("info_player_start", pSpot);
        } else if (m_iTeam == TERRORIST) {
            pSpot = g_pLastTerroristSpawn;
            SelectSpawnSpot("info_player_deathmatch", pSpot);
        } else {
            return;
        }

        if (FNullEnt(pSpot))
        {
            ALERT(at_error, "PutClientInServer: no info_player_start on level\n");
            return INDEXENT(0);
        }

        if (m_iTeam == TERRORIST)
            g_pLastTerroristSpawn = pSpot;
        else
            g_pLastCTSpawn = pSpot;

        return pSpot->edict();
    }
    */

    auto TeamGameRules::GetPlayerRelationship(PlayerBase* player, EntityBase* target) -> PlayerRelationship {
        if (player == target) {
            return PlayerRelationship::Teammate;
        }
        if (!player || !target) {
            return PlayerRelationship::NotTeammate;
        }
        if (!target->IsPlayer()) {
            return PlayerRelationship::NotTeammate;
        }
        if (player->team != static_cast<PlayerBase*>(target)->team) {
            return PlayerRelationship::NotTeammate;
        }
        return PlayerRelationship::Teammate;
    }

    auto TeamGameRules::InitializePlayerCounts(int& aliveTr, int& aliveCt, int& deadTr, int& deadCt) -> void {
        enum {
            DEAD,
            ALIVE,
            MAX_STATUS,
        };

        int playersCount[MAX_TEAMS][MAX_STATUS] = {};
        Players.forEachConnected([&playersCount](const auto& player) {
            ++playersCount[toInt(player.getTeam())][player.isAlive()];
        });
        constexpr auto TEAM_HUMAN = static_cast<int>(Team::Human);
        constexpr auto TEAM_ZOMBIE = static_cast<int>(Team::Zombie);
        aliveTr = playersCount[TEAM_ZOMBIE][ALIVE];
        aliveCt = playersCount[TEAM_HUMAN][ALIVE];
        deadTr = playersCount[TEAM_ZOMBIE][DEAD];
        deadCt = playersCount[TEAM_HUMAN][DEAD];
    }

    auto TeamGameRules::ShouldSwitchWeapon(PlayerBase* base, PlayerItemBase* weapon) -> qboolean {
        if (!weapon->CanDeploy()) {
            return false;
        }
        const auto& player = Players[base];
        const auto activeItem = player.getActiveItem();
        if (activeItem == nullptr) {
            return true;
        }
        if (base->auto_wep_switch == 0 ||
            base->auto_wep_switch == 2 && (player.getButtonLast() & (IN_ATTACK | IN_ATTACK2))) {
            return false;
        }
        if (!activeItem->CanHolster()) {
            return false;
        }
        const auto weaponItemInfo = weapon->GetCsPlayerItem()->item_info;
        const auto activeItemInfo = activeItem->GetCsPlayerItem()->item_info;
        if (player.getWaterLevel() == 3) {
            if (weaponItemInfo.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER) {
                return false;
            }
            if (activeItemInfo.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER) {
                return true;
            }
        }
        if (weaponItemInfo.weight > activeItemInfo.weight) {
            return true;
        }
        return false;
    }

    auto TeamGameRules::GetNextBestWeapon(PlayerBase* base, PlayerItemBase* weapon) -> qboolean {
        if (!weapon->CanHolster()) {
            return false;
        }
        auto& player = Players[base];
        const auto inWater = (player.getWaterLevel() == 3);
        auto bestWeight = -1;
        PlayerItemBase* best = nullptr;
        for (int i = 0; i < MAX_ITEM_TYPES; i++) {
            auto candidate = player.getPlayerItems(i);
            while (candidate != nullptr) {
                const auto itemInfo = candidate->GetCsPlayerItem()->item_info;
                if (candidate != weapon && candidate->CanDeploy() && itemInfo.weight > bestWeight) {
                    if (!(inWater && (itemInfo.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER))) {
                        bestWeight = itemInfo.weight;
                        best = candidate;
                    }
                }
                candidate = candidate->next;
            }
        }
        if (best == nullptr) {
            return false;
        }
        player.SwitchWeapon(best);
        return true;
    }

    // TODO cache file string
    auto TeamGameRules::SendMotdToClient(const Player& player) -> void {
        sendServerName(player, CvarGetString("hostname"));
        auto length = 0;
        const auto motdFile = LoadFileForMe(CvarGetString("motdfile"), &length);
        if (motdFile == nullptr) {
            return;
        }
        const auto text = std::string{reinterpret_cast<char*>(motdFile)};
        UTIL_SendMotd(player, text);
        FreeFile(motdFile);
    }
}
