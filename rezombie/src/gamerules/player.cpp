#include "rezombie/gamerules/game_rules.h"
#include <messages/engine_message.h>
#include "rezombie/player/player_preview.h"
#include "rezombie/player/players.h"
#include "rezombie/entity/weather.h"
#include "rezombie/util.h"
#include <cssdk/public/regamedll/cs_player_item.h>
#include <metamod/engine.h>
#include <metamod/gamedll.h>
#include <metamod/utils.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;
    using namespace metamod::gamedll;
    using namespace player;
    using namespace message;

    auto TeamPlayGameRules::ClientConnected(Edict* entity, const char* name, const char* address, char* rejectReason)
      -> qboolean
    {
        return g_originalGameRules->CsGameRules()->ClientConnected(entity, name, address, rejectReason);
    }

    auto TeamPlayGameRules::ClientCommand(PlayerBase* player, const char* command) -> qboolean
    {
        return g_originalGameRules->CsGameRules()->ClientCommand(player, command);
    }

    auto TeamPlayGameRules::ClientCommandDeadOrAlive(PlayerBase* player, const char* command) -> qboolean
    {
        return g_originalGameRules->CsGameRules()->ClientCommand(player, command);
    }

    auto TeamPlayGameRules::ClientUserInfoChanged(PlayerBase* player, char* infoBuffer) -> void
    {
        // player->CSPlayer()->SetPlayerModel(false);
        // pPlayer->SetPrefsFromUserinfo(infoBuffer);
    }

    auto TeamPlayGameRules::ClientDisconnected(Edict* client) -> void
    {
        g_originalGameRules->CsGameRules()->ClientDisconnected(client);
        /* if (client == nullptr) {
            CheckWinConditions();
            return;
        }
        auto basePlayer = EntityPrivateData<PlayerBase>(client);
        if (basePlayer == nullptr) {
            CheckWinConditions();
            return;
        }
        auto& player = players[basePlayer];
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

    auto TeamPlayGameRules::InitHud(PlayerBase* base) -> void
    {
        const auto& player = players[base];
        netLightStyle(player, 0, g_weather.getLight());
        sendFog(player, g_weather.getFogColor(), g_weather.getFogDensity());
        if (g_weather.getWeatherType() != WeatherType::None) {
            sendReceiveW(player, g_weather.getWeatherType());
        }
        UpdateGameMode(player);
        sendViewMode(player);
        sendScoreInfo(player, player.id(), 0, 0, 0, player.getTeamId());
        sendShadowIdx(player, shadowSprite_);
        sendTeamScore(player, Team::Human, getTeamWins(Team::Human));
        sendTeamScore(player, Team::Zombie, getTeamWins(Team::Zombie));
        sendForceCam(player, mp_forcecamera->value != 0, mp_forcechasecam->value != 0, mp_fadetoblack->value != 0);
        players.forEachConnected(
          [&player](const auto& target)
          {
              sendScoreInfo(player, target.id(), target.getFrags(), target.getDeaths(), 0, target.getTeamId());
          }
        );
        if (getGameState() != GameState::Over) {
            SendMotdToClient(player);
        }
        players.forEachConnected(
          [&player](const auto& target)
          {
              sendTeamInfo(player, target.id(), target.getTeam());
              // target->GetCsPlayer()->SetScoreAttribute(player); // SetScoreboardAttributeSSS(player);
              sendHealthInfo(player, target.id(), target.getHealth());
              sendAccount(player, target.id(), target.getAccount());
              if (player.id() != target.id() && target.isAlive()) {
                  sendRadar(player, target.id(), target.getOrigin());
              }
          }
        );
    }

    auto TeamPlayGameRules::UpdateGameMode(PlayerBase* base) -> void
    {
        const auto& player = players[base];
        sendGameMode(player, 1);
    }

    auto TeamPlayGameRules::PlayerFallDamage(PlayerBase* player) -> float
    {
        if (!mp_falldamage->value) {
            return 0;
        }
        player->fall_velocity -= MAX_PLAYER_SAFE_FALL_SPEED;
        return player->fall_velocity * DAMAGE_FOR_FALL_SPEED * 1.25;
    }

    auto TeamPlayGameRules::PlayerCanTakeDamage(PlayerBase* player, EntityBase* attacker) -> qboolean
    {
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

    auto TeamPlayGameRules::PlayerThink(PlayerBase* base) -> void
    {
        auto& player = players[base];
        if (!gameRules->isCanMove()) {
            player.setCanShoot(false);
            player.setButton(0);
            player.setButtonPressed(0);
            player.setButtonReleased(0);
            return;
        }
        player.setCanShoot(true);
        if (player.getWaterLevel() == 3) {
            const auto activeItem = player.getActiveItem();
            if (activeItem != nullptr && activeItem->IsWeapon() && activeItem->GetCsPlayerItem()->item_info.flags & ITEM_FLAG_NO_FIRE_UNDER_WATER) {
                player.setCanShoot(false);
            }
        }
        if (player.getJoiningState() == JoinState::ShowTeamSelect) {
            player.setJoiningState(JoinState::PickingTeam);
            if (player.getMenu() != MenuName::ChooseAppearance) {
                if (player.isBot()) {
                    player.SelectAppearance();
                } else {
                    player.ShowAppearance();
                }
            }
        }
    }

    auto TeamPlayGameRules::PlayerSpawn(PlayerBase* base) -> void
    {
        auto& player = players[base];
        if (player.isJustConnected()) {
            return;
        }
        const auto autoWeaponSwitch = base->auto_wep_switch;
        base->auto_wep_switch = 1;
        player.setWeapons(player.getWeapons() | (1 << WEAPON_SUIT));
        player.SpawnEquip();
        base->auto_wep_switch = autoWeaponSwitch;
    }

    auto TeamPlayGameRules::PlayerCanRespawn(PlayerBase* base) -> qboolean
    {
        if (getGameState() == GameState::Over) {
            return false;
        }
        if (getRoundState() == RoundState::Terminate) {
            return false;
        }
        // auto& player = players[base];
        // if (player.getMenu() == MenuName::ChooseAppearance) {
        //     return false;
        // }
        return true;
    }

    auto TeamPlayGameRules::PlayerKilled(PlayerBase* victimBase, EntityVars* killerVars, EntityVars* inflictor) -> void
    {
        auto& victim = players[victimBase];
        DeathNotice(victimBase, killerVars, inflictor);
        //// victimBase->m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
        victim.setKilled(true);
        victim.setDeaths(victim.getDeaths() + 1);
        //// victimBase->m_iTrain = (TRAIN_NEW | TRAIN_OFF);
        // SetView(ENT(victimBase->vars), ENT(victimBase->vars));
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
                auto& killer = players[killerPlayer];
                killer.setFrags(killer.getFrags() + PointsForKill(killer, victim));
                killer.AddAccount(REWARD_KILLED_ENEMY, RewardType::EnemyKilled);
                // FireTargets("game_playerkill", killerPlayer, killerPlayer, USE_TOGGLE, 0);
            }
        } else {
            killerVars->frags -= 1;
        }
        sendScoreInfo(nullptr, victim.id(), victim.getFrags(), victim.getDeaths(), 0, victim.getTeamId());
        if (killerPlayer != nullptr && killerPlayer->IsPlayer()) {
            auto& killer = players[killerEntity->EdictIndex()];
            sendScoreInfo(TO_ALL, killer.id(), killer.getFrags(), killer.getDeaths(), 0, killer.getTeamId());
            killer.setNextSprayTime(0.0);
        }
    }

    auto TeamPlayGameRules::PointsForKill(PlayerBase*, PlayerBase*) -> int
    {
        return 1;
    }

    auto TeamPlayGameRules::DeathNotice(PlayerBase* victimBase, EntityVars* killerVars, EntityVars* inflictorVars)
      -> void
    {
        const auto& victim = players[victimBase];
        auto killerIndex = 0;
        std::string_view weaponName{"world"};
        if (killerVars->flags & FL_CLIENT) {
            const auto killerEdict = killerVars->containing_entity;
            killerIndex = IndexOfEdict(killerEdict);
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
        sendDeathMsg(TO_ALL, killerIndex, victim.id(), victim.isHeadshotKilled(), weaponName.data());
    }

    auto TeamPlayGameRules::GetPlayerSpawnSpot(PlayerBase* base) -> Edict*
    {
        auto& player = players[base];
        auto spawnSpot = base->GetCsPlayer()->EntSelectSpawnPoint(); // to be removed
        player.setOrigin(spawnSpot->vars.origin + Vector(0, 0, 1));
        player.setViewAngle(VECTOR_ZERO);
        player.setVelocity(VECTOR_ZERO);
        player.setAngles(spawnSpot->vars.angles);
        player.setPunchAngle(VECTOR_ZERO);
        player.setFixAngleMode(FixAngleMode::ForceViewAngles);
        /*if (spawnSpot->v.target) {
            FireTargets(STRING(pentSpawnSpot->v.target), pPlayer, pPlayer, USE_TOGGLE, 0);
        }*/
        return spawnSpot;
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

    auto TeamPlayGameRules::GetPlayerRelationship(PlayerBase* player, EntityBase* target) -> PlayerRelationship
    {
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

    auto TeamPlayGameRules::InitializePlayerCounts(int& aliveTr, int& aliveCt, int& deadTr, int& deadCt) -> void
    {
        enum
        {
            DEAD,
            ALIVE,
            MAX_STATUS,
        };

        int playersCount[MAX_TEAMS][MAX_STATUS] = {};
        const auto countPlayers = [&playersCount](const auto& player)
        {
            ++playersCount[player.getTeamId()][player.isAlive()];
        };
        players.forEachConnected(countPlayers);
        constexpr auto TEAM_HUMAN = static_cast<int>(Team::Human);
        constexpr auto TEAM_ZOMBIE = static_cast<int>(Team::Zombie);
        aliveTr = playersCount[TEAM_ZOMBIE][ALIVE];
        aliveCt = playersCount[TEAM_HUMAN][ALIVE];
        deadTr = playersCount[TEAM_ZOMBIE][DEAD];
        deadCt = playersCount[TEAM_HUMAN][DEAD];
        setPlayersCount(Team::Human, aliveCt + deadCt);
        setPlayersCount(Team::Zombie, aliveTr + deadTr);
    }

    auto TeamPlayGameRules::CountPlayers() -> void
    {
        int playersCount[MAX_TEAMS] = {};
        const auto countPlayers = [&playersCount](const auto& player)
        {
            ++playersCount[player.getTeamId()];
        };
        players.forEachConnected(countPlayers);
        setPlayersCount(Team::Human, playersCount[toInt(Team::Human)]);
        setPlayersCount(Team::Zombie, playersCount[toInt(Team::Zombie)]);
    }

    auto TeamPlayGameRules::ShouldSwitchWeapon(PlayerBase* base, PlayerItemBase* weapon) -> qboolean
    {
        if (!weapon->CanDeploy()) {
            return false;
        }
        const auto& player = players[base];
        const auto activeItem = player.getActiveItem();
        if (activeItem == nullptr) {
            return true;
        }
        if (base->auto_wep_switch == 0 || base->auto_wep_switch == 2 && (player.getButtonLast() & (IN_ATTACK | IN_ATTACK2))) {
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

    auto TeamPlayGameRules::GetNextBestWeapon(PlayerBase* base, PlayerItemBase* weapon) -> qboolean
    {
        if (!weapon->CanHolster()) {
            return false;
        }
        auto& player = players[base];
        const auto inWater = (player.getWaterLevel() == 3);
        auto bestWeight = -1;
        PlayerItemBase* candidate = nullptr;
        PlayerItemBase* best = nullptr;
        for (int i = 0; i < MAX_ITEM_TYPES; i++) {
            candidate = player.getPlayerItems(i);
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

    auto TeamPlayGameRules::SendMotdToClient(const Player& player) -> void
    {
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
