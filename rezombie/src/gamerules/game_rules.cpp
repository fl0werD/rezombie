#include "rezombie/gamerules/game_rules.h"
#include "rezombie/gamerules/api/game_rules.h"
#include "rezombie/gamerules/api/modes.h"
#include "rezombie/gamerules/modules/modes.h"
#include "rezombie/player/players.h"
#include "rezombie/map/environment.h"
#include "rezombie/main/util.h"
#include "rezombie/messages/user_message.h"
#include <core/strings/format.h>
#include <metamod/engine.h>
#include <metamod/gamedll.h>
#include <mhooks/reapi.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;
    using namespace metamod::gamedll;
    using namespace mhooks;

    auto RegisterGameRulesHooks() -> void {
        MHookReGameRulesInstallGameRules(DELEGATE_ARG<InstallGameRules>, HookChainPriority::Low);
        MHookReGameRulesFreeGameRules(DELEGATE_ARG<FreeGameRules>);
    }

    auto InstallGameRules(const ReGameRulesInstallGameRulesMChain& chain) -> cssdk::GameRules* {
        OriginalGameRules = chain.CallNext();
        ConVarInit();
        if (!gameRules) {
            gameRules = new TeamPlayGameRules();
        }
        return gameRules;
    }

    auto FreeGameRules(const ReGameRulesFreeGameRulesMChain& chain, cssdk::GameRules**) -> void {
        chain.CallNext(&OriginalGameRules);
        OriginalGameRules = nullptr;
        delete gameRules;
        gameRules = nullptr;
    }

    TeamPlayGameRules::TeamPlayGameRules() {
        // setGameState(GameState::Warmup);
        // setRoundRemainingTime(10);
        roundRemainingTime_ = 10;
        game_started_ = false;
        freeze_period = false;
        setWinStatus(WinStatus::None);

        // m_VoiceGameMgr.Init(&g_GameMgrHelper, MaxClients());

        setTeamWins(Team::Human, 0, false);
        setTeamWins(Team::Zombie, 0, false);

        setReset(false);

        t_cant_buy = true;
        ct_cant_buy = true;

        round_terminating = false;
        setRoundsPlayed(0);

        ReadMultiplayCvars();

        max_idle_period = std::numeric_limits<float>::max();

        CvarSetFloat("pausable", 0);

        skip_spawn_ = false;
        skip_show_menu_ = false;
        time_limit_ = 0;

        spawn_point_count_terrorist = g_global_vars->max_clients;
        spawn_point_count_ct = g_global_vars->max_clients;
        level_initialized = true;

        // change to listener
        CvarSetFloat(sv_accelerate->name, 5);
        CvarSetFloat(sv_friction->name, 4);
        CvarSetFloat(sv_stopspeed->name, 75);

        Environment.reset();
    }

    auto TeamPlayGameRules::ReadMultiplayCvars() -> void {
        //limit_teams = 0;
        //unbalanced_rounds = 0;
        //CvarSetFloat(mp_limitteams->name, 0);
        //CvarSetFloat(mp_autoteambalance->name, 0);
        round_time = std::clamp(3 * 60, 0, 30000);
        CheckRoundLimits();
    }

    auto TeamPlayGameRules::CheckRoundLimits() -> void {
        max_rounds = static_cast<int>(mp_maxrounds->value);
        if (max_rounds < 0) {
            max_rounds = 0;
            CvarSetFloat(mp_maxrounds->name, 0);
        }

        max_rounds_won = static_cast<int>(mp_winlimit->value);
        if (max_rounds_won < 0) {
            max_rounds_won = 0;
            CvarSetFloat(mp_winlimit->name, 0);
        }
    }

    auto TeamPlayGameRules::RestartRound() -> void {
        // change to listener
        CvarSetFloat(sv_accelerate->name, 5);
        CvarSetFloat(sv_friction->name, 4);
        CvarSetFloat(sv_stopspeed->name, 75);

        if (isReset()) {
            if (mp_timelimit->value < 0) {
                CvarSetFloat(mp_timelimit->name, 0);
            } else if (mp_timelimit->value) {
                time_limit_ = g_global_vars->time + (mp_timelimit->value * 60);
            }
            game_start_time_ = g_global_vars->time;
            setRoundsPlayed(0);
            setTeamWins(Team::Human, 0);
            setTeamWins(Team::Zombie, 0);
            Players.forEachConnected(
                [](auto& player) {
                    player.Reset();
                }
            );
        } else {
            setRoundsPlayed(getRoundsPlayed() + 1);
        }

        ReadMultiplayCvars();
        CleanUpMap();

        setLastMode(getMode());
        setMode(0);
        setDefaultPlayerClassOverride(Team::Human, 0);
        setDefaultPlayerClassOverride(Team::Zombie, 0);
        setWinStatus(WinStatus::None);

        if (getGameState() == GameState::Playing) {
            setRoundState(RoundState::Prepare);
        }

        Players.forEachConnected(
            [](auto& player) {
                player.setNumSpawns(0);
                if (!player.isPlayableTeam()) {
                    return;
                }
                player.RoundRespawn();
            }
        );

        if (getGameState() == GameState::Playing) {
            setRoundRemainingTime(10);
        }

        Environment.reset();

        GameRulesApi.RoundStart(isReset());
        setReset(false);
    }

    auto TeamPlayGameRules::Think() -> void {
        // m_VoiceGameMgr.Update(gpGlobals->frametime);

        if (getGameState() != GameState::Warmup && getGameState() != GameState::Over) {
            if (mp_timelimit->value < 0) {
                CvarSetFloat(mp_timelimit->name, 0);
                return;
            } else {
                if (mp_timelimit->value != 0.f) {
                    time_limit_ = game_start_time_ + mp_timelimit->value * 60;
                    if (g_global_vars->time >= time_limit_) {
                        GoToIntermission();
                        return;
                    }
                }
                static int lastTime = 0;
                int timeRemaining = mp_timelimit->value ? static_cast<int>(time_limit_ - g_global_vars->time) : 0;
                if (timeRemaining != lastTime) {
                    lastTime = timeRemaining;
                    CvarDirectSet(
                        mp_timeleft, core::str::Format("%02d:%02d", timeRemaining / 60, timeRemaining % 60).c_str()
                    );
                }
            }
        }

        if (getGameState() != GameState::Warmup && getGameState() != GameState::Over) {
            int aliveTr, aliveCt, deadTr, deadCt;
            InitializePlayerCounts(aliveTr, aliveCt, deadTr, deadCt);
            if (getPlayersCount(PlayersCountFlags::Humans | PlayersCountFlags::Zombies) >= 2) {
                switch (getGameState()) {
                    case GameState::NeedPlayers: {
                        EndRound(EndRoundEvent::GameCommence);
                        break;
                    }
                    case GameState::Playing: {
                        if (getRoundState() == RoundState::Playing) {
                            if (!aliveTr) {
                                EndRound(EndRoundEvent::HumansWin);
                            } else if (!aliveCt) {
                                EndRound(EndRoundEvent::ZombiesWin);
                            }
                        }
                        break;
                    }
                }
            } else {
                if (getGameState() != GameState::NeedPlayers) {
                    setGameState(GameState::NeedPlayers);
                    setRoundState(RoundState::None);
                }
            }
        }

        if (sv_restart->value != 0.f && getGameState() != GameState::Over) {
            EndRound(EndRoundEvent::GameRestart);
            return;
        }

        if (nextRoundTimeUpdateTime_ <= g_global_vars->time) {
            nextRoundTimeUpdateTime_ = g_global_vars->time + 1.0f;
            setRoundRemainingTime(getRoundRemainingTime() - 1);
        }
        if (getRoundRemainingTime() != 0) {
            return;
        }
        switch (getGameState()) {
            case GameState::Warmup: {
                if (getRoundState() != RoundState::Terminate) {
                    EndRound(EndRoundEvent::WarmupEnd);
                    Players.forEachConnected(
                        [](auto& player) {
                            if (!player.isAlive()) {
                                return;
                            }
                            player.setIUser3(player.getIUser3() | NO_MOVE_PLAYER_FLAGS);
                            player.ResetMaxSpeed();
                        }
                    );
                } else {
                    if (getPlayersCount(PlayersCountFlags::Humans | PlayersCountFlags::Zombies) < 2) {
                        setGameState(GameState::NeedPlayers);
                        setRoundState(RoundState::None);
                    } else {
                        setGameState(GameState::Playing);
                    }
                    RestartRound();
                }
                break;
            }
            case GameState::NeedPlayers: {
                // No op
                break;
            }
            case GameState::Playing: {
                switch (getRoundState()) {
                    case RoundState::Prepare: {
                        const auto modeId = getRandomMode();
                        const auto modeRef = Modes[modeId];
                        if (modeRef) {
                            setMode(modeId);
                            const auto& mode = modeRef->get();
                            if (mode.getRoundTime()) {
                                setRoundRemainingTime(mode.getRoundTime());
                            } else {
                                setRoundRemainingTime(round_time);
                            }
                            mode.executeLaunch(modeId);
                        }
                        // TODO: no need?
                        ModesApi.ModeStart(modeId);
                        setRoundState(RoundState::Playing);
                        break;
                    }
                    case RoundState::Playing: {
                        EndRound(EndRoundEvent::HumansWin);
                        break;
                    }
                    case RoundState::Terminate: {
                        RestartRound();
                        break;
                    }
                }
                break;
            }
            case GameState::Over: {
                ChangeLevel();
                break;
            }
        }
    }

    auto TeamPlayGameRules::getRandomMode() const -> int {
        if (Modes.count() == 1) {
            return Modes.begin();
        }
        auto total = 0.f;
        std::unordered_map<int, float> indexAndChance;
        auto playersCount = getPlayersCount();
        Modes.forEachIndexed(
            [&](int index, Mode& item) {
                if (playersCount < item.getMinPlayers()) {
                    return;
                }
                const auto chance = static_cast<float>(item.getDropChance()) / 100;
                indexAndChance[index] = chance;
                total += chance;
            }
        );
        if (indexAndChance.size() == 1) {
            return indexAndChance.begin()->first;
        }
        const auto chance = total * RandomFloat(0.0, 1.0);
        auto current = 0.f;
        auto resultMode = 0;
        std::all_of(
            indexAndChance.cbegin(), indexAndChance.cend(),
            [&](const auto& mode) {
                if (current <= chance && chance < current + mode.second) {
                    resultMode = mode.first;
                    return false;
                }
                current += mode.second;
                return true;
            }
        );
        return resultMode;
    }

    auto TeamPlayGameRules::EndRound(EndRoundEvent event) -> void {
        switch (event) {
            case EndRoundEvent::WarmupEnd: {
                setReset(true);
                // setGameState(GameState::Playing);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setRoundRemainingTime(3);
                break;
            }
            case EndRoundEvent::GameCommence: {
                setReset(true);
                setGameState(GameState::Playing);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setRoundRemainingTime(3);
                break;
            }
            case EndRoundEvent::GameRestart: {
                CvarSetFloat(sv_restart->name, 0);
                setReset(true);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setRoundRemainingTime(1);
                break;
            }
            case EndRoundEvent::GameOver: {
                setGameState(GameState::Over);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setRoundRemainingTime(10);
                break;
            }
            case EndRoundEvent::HumansWin: {
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Cts);
                setTeamWins(Team::Human, getTeamWins(Team::Human) + 1);
                setRoundRemainingTime(static_cast<int>(mp_round_restart_delay->value));
                CheckRoundsLimit();
                break;
            }
            case EndRoundEvent::ZombiesWin: {
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Terrorists);
                setTeamWins(Team::Zombie, getTeamWins(Team::Zombie) + 1);
                setRoundRemainingTime(static_cast<int>(mp_round_restart_delay->value));
                CheckRoundsLimit();
                break;
            }
        }
        GameRulesApi.RoundEnd(event, getMode(), getRoundRemainingTime());
    }

    auto TeamPlayGameRules::UpdateTeamScores() -> void {
        sendTeamScore(TO_ALL, Team::Human, getTeamWins(Team::Human));
        sendTeamScore(TO_ALL, Team::Zombie, getTeamWins(Team::Zombie));
    }

    auto TeamPlayGameRules::GoToIntermission() -> void {
        if (getGameState() == GameState::Over) {
            return;
        }
        int chatTime = std::clamp(static_cast<int>(mp_chattime->value), 0, MAX_INTERMISSION_TIME);
        setGameState(GameState::Over); // endRound?
        setRoundRemainingTime(chatTime);
        Players.forEachConnected(
            [](auto& player) {
                if (!(player.getButton() & IN_SCORE)) {
                    metamod::engine::ClientCommand(player, "togglescores");
                }
                player.setIUser3(player.getIUser3() | NO_MOVE_PLAYER_FLAGS);
                player.ResetMaxSpeed();
            }
        );
    }

    auto TeamPlayGameRules::ChangeLevel() -> void {
        const char* nextMap = "de_dust2";
        metamod::engine::ChangeLevel(nextMap, nullptr);
    }

    auto TeamPlayGameRules::CleanUpMap() -> void {
        OriginalGameRules->CsGameRules()->CleanUpMap();
    }

    auto TeamPlayGameRules::RemoveGuns() -> void {
        OriginalGameRules->CsGameRules()->RemoveGuns();
    }

    auto TeamPlayGameRules::CheckWinConditions() -> void {
        /*if (getGameState() == GameState::Warmup || getGameState() == GameState::Over) {
            return;
        }
        int aliveTr, aliveCt, deadTr, deadCt;
        InitializePlayerCounts(aliveTr, aliveCt, deadTr, deadCt);
        if (getPlayersCount(PlayersCountFlags::Humans | PlayersCountFlags::Zombies) < 2) {
            setGameState(GameState::NeedPlayers);
            setRoundState(RoundState::None);
            return;
        }
        switch (getGameState()) {
            case GameState::NeedPlayers: {
                EndRound(EndRoundEvent::GameCommence);
                break;
            }
            case GameState::Playing: {
                if (getRoundState() == RoundState::Playing) {
                    if (!aliveTr) {
                        EndRound(EndRoundEvent::HumansWin);
                    } else if (!aliveCt) {
                        EndRound(EndRoundEvent::ZombiesWin);
                    }
                }
                break;
            }
        }*/
    }

    auto TeamPlayGameRules::CheckRoundsLimit() -> bool {
        if (max_rounds_won != 0 &&
            (getTeamWins(Team::Human) >= max_rounds_won || getTeamWins(Team::Zombie) >= max_rounds_won)) {
            GoToIntermission();
            return true;
        }
        if (max_rounds != 0 && getRoundsPlayed() >= max_rounds) {
            GoToIntermission();
            return true;
        }
        return false;
    }

    auto TeamPlayGameRules::changeGameState(GameState gameState) -> void {
        /*if (getGameState() == gameState) {
            return;
        }*/
        setNextGameState(gameState);
        switch (gameState) {
            case GameState::Warmup: {
                break;
            }
            case GameState::NeedPlayers: {
                break;
            }
            case GameState::Playing: {
                break;
            }
            case GameState::Over: {
                break;
            }
        }


        setGameState(getNextGameState());
        switch (getGameState()) {
            case GameState::Warmup: {
                break;
            }
            case GameState::NeedPlayers: {
                break;
            }
            case GameState::Playing: {
                break;
            }
            case GameState::Over: {
                break;
            }
        }
    }

    auto TeamPlayGameRules::GetTimeLeft() const {
        return time_limit_ - g_global_vars->time;
    }

    auto TeamPlayGameRules::GetRoundElapsedTime() const {
        return g_global_vars->time - round_start_time;
    }

    auto TeamPlayGameRules::GetMapElapsedTime() const {
        return g_global_vars->time;
    }

    auto TeamPlayGameRules::GetRoundRespawnTime() const {
        return mp_roundrespawn_time->value;
    }

    auto TeamPlayGameRules::getTeamWins(Team team) const -> short {
        switch (team) {
            case Team::Human: {
                return num_ct_wins;
            }
            case Team::Zombie: {
                return num_terrorist_wins;
            }
            default: {
                return 0;
            }
        }
    }

    auto TeamPlayGameRules::setTeamWins(Team team, int wins, bool update) -> void {
        switch (team) {
            case Team::Human: {
                num_ct_wins = static_cast<short>(wins);
                if (update) {
                    sendTeamScore(TO_ALL, team, getTeamWins(team));
                }
                break;
            }
            case Team::Zombie: {
                num_terrorist_wins = static_cast<short>(wins);
                if (update) {
                    sendTeamScore(TO_ALL, team, getTeamWins(team));
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    auto TeamPlayGameRules::getWinStatus() const -> WinStatus {
        return round_win_status;
    }

    auto TeamPlayGameRules::setWinStatus(WinStatus winStatus) -> void {
        round_win_status = winStatus;
    }

    auto TeamPlayGameRules::isReset() const -> bool {
        return complete_reset;
    }

    auto TeamPlayGameRules::setReset(bool isReset) -> void {
        complete_reset = isReset;
    }

    auto TeamPlayGameRules::getRoundsPlayed() const -> int {
        return total_rounds_played;
    }

    auto TeamPlayGameRules::setRoundsPlayed(int roundsPlayed) -> void {
        total_rounds_played = roundsPlayed;
    }

    auto TeamPlayGameRules::getRoundRemainingTime() const -> int {
        return roundRemainingTime_;
    }

    auto TeamPlayGameRules::setRoundRemainingTime(int remainingTime) -> void {
        if (remainingTime < 0) {
            remainingTime = 0;
        }
        roundRemainingTime_ = remainingTime;
        nextRoundTimeUpdateTime_ = g_global_vars->time + 1.0f;
        GameRulesApi.RoundTimer(getRoundRemainingTime());
    }

    auto TeamPlayGameRules::getGameState() const -> GameState {
        return gameState_;
    }

    auto TeamPlayGameRules::setGameState(GameState gameState) -> void {
        const auto oldState = gameState_;
        gameState_ = gameState;
        GameRulesApi.GameStateChanged(oldState, gameState);
    }

    auto TeamPlayGameRules::getNextGameState() const -> GameState {
        return nextGameState_;
    }

    auto TeamPlayGameRules::setNextGameState(GameState gameState) -> void {
        nextGameState_ = gameState;
    }

    auto TeamPlayGameRules::getRoundState() const -> RoundState {
        return roundState_;
    }

    auto TeamPlayGameRules::setRoundState(RoundState roundState) -> void {
        const auto oldState = roundState_;
        roundState_ = roundState;
        GameRulesApi.RoundStateChanged(oldState, roundState);
    }

    auto TeamPlayGameRules::getNextRoundState() const -> RoundState {
        return roundState_;
    }

    auto TeamPlayGameRules::setNextRoundState(RoundState roundState) -> void {
        nextRoundState_ = roundState;
    }

    auto TeamPlayGameRules::getMode() const -> int {
        return modeId_;
    }

    auto TeamPlayGameRules::setMode(int modeId) -> void {
        modeId_ = modeId;
    }

    auto TeamPlayGameRules::getLastMode() const -> int {
        return lastModeId_;
    }

    auto TeamPlayGameRules::setLastMode(int lastModeId) -> void {
        lastModeId_ = lastModeId;
    }

    auto TeamPlayGameRules::getDefaultPlayerClass(Team team) const -> int {
        return defaultPlayerClass_[toInt(team)];
    }

    auto TeamPlayGameRules::setDefaultPlayerClass(Team team, int playerClass) -> void {
        defaultPlayerClass_[toInt(team)] = playerClass;
    }

    auto TeamPlayGameRules::getDefaultPlayerClassOverride(Team team) const -> int {
        return defaultPlayerClassOverride_[toInt(team)];
    }

    auto TeamPlayGameRules::setDefaultPlayerClassOverride(Team team, int playerClass) -> void {
        defaultPlayerClassOverride_[toInt(team)] = playerClass;
    }

    auto TeamPlayGameRules::defaultPlayerClass(Team team) const -> int {
        if (!getDefaultPlayerClassOverride(team)) {
            return getDefaultPlayerClass(team);
        }
        return getDefaultPlayerClassOverride(team);
    }

    auto TeamPlayGameRules::isCanMove() const -> bool {
        switch (getGameState()) {
            case GameState::Warmup: {
                if (getRoundState() == RoundState::Terminate) {
                    return false;
                }
                break;
            }
            case GameState::Over: {
                return false;
            }
        }
        return true;
    }

    auto TeamPlayGameRules::precache() -> void {
        shadowSprite_ = PrecacheModel("sprites/shadow_circle.spr");
    }
}
