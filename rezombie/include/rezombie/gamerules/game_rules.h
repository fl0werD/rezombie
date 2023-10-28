#pragma once

#include "rezombie/main/convars.h"
#include "rezombie/player/player.h"
#include <cssdk/dll/game_rules.h>
#include <mhooks/reapi.h>

namespace rz
{
    using namespace cssdk;

    auto RegisterGameRulesHooks() -> void;

    auto InstallGameRules(const ReGameRulesInstallGameRulesMChain& chain) -> GameRules*;

    auto FreeGameRules(const ReGameRulesFreeGameRulesMChain& chain, GameRules**) -> void;

    class TeamPlayGameRules : public HalfLifeMultiplay {
      public:
        TeamPlayGameRules();

        ~TeamPlayGameRules() override = default;

        auto RefreshSkillData() -> void override {};

        auto Think() -> void override;

        auto IsAllowedToSpawn(EntityBase*) -> qboolean override { return true; }

        auto AllowFlashlight() -> qboolean override { return mp_flashlight->value != 0; }

        auto ShouldSwitchWeapon(PlayerBase* player, PlayerItemBase* weapon) -> qboolean override;
        auto GetNextBestWeapon(PlayerBase* player, PlayerItemBase* currentWeapon) -> qboolean override;

        auto IsMultiplayer() -> qboolean override { return true; }

        auto IsDeathmatch() -> qboolean override { return true; }

        auto IsTeamplay() -> qboolean override { return true; }

        auto IsCoop() -> qboolean override { return false; }

        auto GetGameDescription() -> const char* override {
            return "ReZombie";
        }

        auto ClientConnected(Edict* entity, const char* name, const char* address, char rejectReason[128])
        -> qboolean override;
        auto InitHud(PlayerBase* player) -> void override;
        auto ClientDisconnected(Edict* client) -> void override;
        auto UpdateGameMode(PlayerBase* player) -> void override;
        auto PlayerFallDamage(PlayerBase* player) -> float override;
        auto PlayerCanTakeDamage(PlayerBase* player, EntityBase* attacker) -> qboolean override;

        auto ShouldAutoAim(PlayerBase*, Edict*) -> qboolean override { return false; }

        auto PlayerSpawn(PlayerBase* player) -> void override;
        auto PlayerThink(PlayerBase* player) -> void override;
        auto PlayerCanRespawn(PlayerBase* player) -> qboolean override;

        auto PlayerSpawnTime(PlayerBase*) -> float override { return 0.f; };

        auto GetPlayerSpawnSpot(PlayerBase* player) -> Edict* override;

        auto AllowAutoTargetCrosshair() -> qboolean override { return false; }

        auto ClientCommandDeadOrAlive(PlayerBase* player, const char* cmd) -> qboolean override;
        auto ClientCommand(PlayerBase* player, const char* cmd) -> qboolean override;
        auto ClientUserInfoChanged(PlayerBase* player, char* infoBuffer) -> void override;
        auto PointsForKill(PlayerBase* attacker, PlayerBase* killed) -> int override;
        auto PlayerKilled(PlayerBase* victim, EntityVars* killer, EntityVars* inflictor) -> void override;
        auto DeathNotice(PlayerBase* victim, EntityVars* killer, EntityVars* inflictor) -> void override;

        auto CanHavePlayerItem(PlayerBase*, PlayerItemBase*) -> qboolean override { return true; }

        auto PlayerGotWeapon(PlayerBase*, PlayerItemBase*) -> void override {};

        auto WeaponShouldRespawn(
            PlayerItemBase*
        ) -> WeaponRespawnCode override { return WeaponRespawnCode::WeaponRespawnNo; }

        auto WeaponRespawnTime(PlayerItemBase*) -> float override { return -1; }

        auto WeaponTryRespawn(PlayerItemBase*) -> float override { return -1; }

        auto WeaponRespawnSpot(PlayerItemBase*) -> Vector override { return VECTOR_ZERO; }

        auto CanHaveItem(PlayerBase*, Item*) -> qboolean override { return false; }

        auto PlayerGotItem(PlayerBase*, Item*) -> void override {};

        auto ItemShouldRespawn(Item*) -> WeaponRespawnCode override { return WeaponRespawnCode::ItemRespawnNo; }

        auto ItemRespawnTime(Item*) -> float override { return -1; }

        auto ItemRespawnSpot(Item*) -> Vector override { return VECTOR_ZERO; }

        auto CanHaveAmmo(PlayerBase*, const char*, int) -> qboolean override { return true; }

        auto PlayerGotAmmo(PlayerBase*, char*, int) -> void override {};

        auto AmmoShouldRespawn(
            PlayerAmmoBase*
        ) -> WeaponRespawnCode override { return WeaponRespawnCode::AmmoRespawnNo; }

        auto AmmoRespawnTime(PlayerAmmoBase*) -> float override { return -1; }

        auto AmmoRespawnSpot(PlayerAmmoBase*) -> Vector override { return VECTOR_ZERO; }

        auto HealthChargerRechargeTime() -> float override { return 0; }

        auto HevChargerRechargeTime() -> float override { return 0; }

        auto DeadPlayerWeapons(PlayerBase*) -> WeaponRespawnCode override {
            return WeaponRespawnCode::PlrDropGunActive;
        }

        auto DeadPlayerAmmo(PlayerBase*) -> WeaponRespawnCode override {
            return WeaponRespawnCode::PlrDropAmmoActive;
        }

        auto GetTeamId(EntityBase*) -> const char* override {
            return "";
        }

        auto GetPlayerRelationship(PlayerBase* player, EntityBase* target) -> PlayerRelationship override;

        auto GetTeamIndex(const char*) -> int override {
            return 0;
        }

        auto GetIndexedTeamName(int) -> const char* override {
            return "";
        }

        auto IsValidTeam(const char*) -> qboolean override {
            return true;
        }

        auto ChangePlayerTeam(PlayerBase*, const char*, qboolean, qboolean) -> void override {
        }

        auto SetDefaultPlayerTeam(PlayerBase*) -> const char* override {
            return "";
        }

        auto PlayTextureSounds() -> qboolean override { return false; };

        auto AllowMonsters() -> qboolean override { return false; }

        auto EndMultiplayerGame() -> void override {};

        auto IsFreezePeriod() -> qboolean override { return false; }

        auto ServerDeactivate() -> void override {};

        auto CheckMapConditions() -> void override {};

        auto CleanUpMap() -> void override;

        auto RestartRound() -> void override;

        auto CheckWinConditions() -> void override;

        auto RemoveGuns() -> void override;

        auto GiveC4() -> void override {};

        auto ChangeLevel() -> void override;

        auto GoToIntermission() -> void override;

        auto InitializePlayerCounts(int& aliveTr, int& aliveCt, int& deadTr, int& deadCt) -> void override;

        auto BalanceTeams() -> void override {};

        auto SwapAllPlayers() -> void override {};

        auto UpdateTeamScores() -> void override;

        auto EndRoundMessage(const char*, ScenarioEventEndRound) -> void override {};

        auto SetAccountRules(RewardRules, int) -> void override {};

        auto GetAccountRules(RewardRules) const -> int override { return 0; }

        auto IsThereABomber() -> qboolean override { return false; }

        auto IsThereABomb() -> qboolean override { return false; }

        auto SelectDefaultTeam() -> TeamName override {
            return TeamName::Ct;
        }

        auto HasRoundTimeExpired() -> bool override { return false; }

        auto IsBombPlanted() -> bool override { return false; }

        auto ReadMultiplayCvars() -> void;
        auto CheckRoundLimits() -> void;

        auto EndRound(EndRoundEvent event) -> void;

        auto CheckTimeLimit() -> bool;
        auto CheckRoundsLimit() -> bool;
        auto CheckRestart() -> bool;

        auto changeGameState(GameState gameState) -> void;
        auto getRandomMode() const -> int;

        auto SendMotdToClient(const Player& player) -> void;

        auto GetTimeLeft() const;
        auto GetRoundElapsedTime() const;
        auto GetMapElapsedTime() const;
        auto GetRoundRespawnTime() const;
        auto getTeamWins(Team team) const -> short;
        auto setTeamWins(Team team, int wins, bool update = true) -> void;
        auto getWinStatus() const -> WinStatus;
        auto setWinStatus(WinStatus winStatus) -> void;
        auto isReset() const -> bool;
        auto setReset(bool isReset) -> void;
        auto getRoundsPlayed() const -> int;
        auto setRoundsPlayed(int roundsPlayed) -> void;
        auto getRoundRemainingTime() const -> int;
        auto setRoundRemainingTime(int remainingTime) -> void;

        auto getGameState() const -> GameState;
        auto setGameState(GameState gameState) -> void;
        auto getNextGameState() const -> GameState;
        auto setNextGameState(GameState gameState) -> void;
        auto getRoundState() const -> RoundState;
        auto setRoundState(RoundState roundState) -> void;
        auto getNextRoundState() const -> RoundState;
        auto setNextRoundState(RoundState roundState) -> void;
        auto getMode() const -> int;
        auto setMode(int modeIid) -> void;
        auto getLastMode() const -> int;
        auto setLastMode(int lastModeId) -> void;
        auto getDefaultPlayerClass(Team team) const -> int;
        auto setDefaultPlayerClass(Team team, int playerClass) -> void;
        auto getDefaultPlayerClassOverride(Team team) const -> int;
        auto setDefaultPlayerClassOverride(Team team, int playerClass) -> void;

        auto defaultPlayerClass(Team team) const -> int;

        auto isCanMove() const -> bool;

        auto precache() -> void;

      private:
        GameState gameState_ = GameState::Warmup;
        GameState nextGameState_ = GameState::Warmup;
        RoundState roundState_ = RoundState::None;
        RoundState nextRoundState_ = RoundState::None;
        int roundRemainingTime_ = 0;
        float nextRoundTimeUpdateTime_ = 0;
        int modeId_ = 0;
        int lastModeId_ = 0;
        int defaultPlayerClass_[MAX_TEAMS] = {0};
        int defaultPlayerClassOverride_[MAX_TEAMS] = {0};

        int shadowSprite_ = 0;
    };

    inline TeamPlayGameRules* gameRules{};
    inline GameRules* OriginalGameRules{};
}
