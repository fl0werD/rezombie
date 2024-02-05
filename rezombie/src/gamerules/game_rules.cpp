#include "rezombie/gamerules/game_rules.h"
#include "rezombie/gamerules/api/game_rules.h"
#include "rezombie/modes/api/modes.h"
#include "rezombie/modes/modules/modes.h"
#include "rezombie/player/players.h"
#include "rezombie/map/environment.h"
#include "rezombie/main/util.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/configs/main_config.h"
#include <core/strings/format.h>
#include <metamod/engine.h>
#include <metamod/gamedll.h>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;
    using namespace metamod::gamedll;

    auto RegisterGameRulesHooks() -> void {
        const auto hooks = regamedll_api::HookChains();

        hooks->InstallGameRules()->RegisterHook(&InstallGameRules, HookChainPriority::Low);
        hooks->FreeGameRules()->RegisterHook(&FreeGameRules);
    }

    auto InstallGameRules(ReHookInstallGameRules* chain) -> GameRulesBase* {
        OriginalGameRules = chain->CallNext();
        ConVarInit();
        /*if (!GameRules) {
            GameRules = new TeamGameRules();
        }*/
        GameRules.Reset();
        return &GameRules;
    }

    auto FreeGameRules(ReHookFreeGameRules* chain, GameRulesBase**) -> void {
        chain->CallNext(&OriginalGameRules);
        OriginalGameRules = nullptr;
        /*delete GameRules;
        GameRules = nullptr;*/
    }

    TeamGameRules::TeamGameRules() {
        // setGameState(GameState::Warmup);
        // setTimer(10);
        timer_ = 10;
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

        max_idle_period = std::numeric_limits<float>::max();

        skip_spawn_ = false;
        skip_show_menu_ = false;
        time_limit_ = 0;

        Environment.reset();
    }

    auto TeamGameRules::Reset() -> void {
        ReadMultiplayCvars();

        CvarSetFloat("pausable", 0);

        spawn_point_count_terrorist = g_global_vars->max_clients;
        spawn_point_count_ct = g_global_vars->max_clients;
        level_initialized = true;

        // change to listener
        CvarSetFloat(sv_accelerate->name, 5);
        CvarSetFloat(sv_friction->name, 4);
        CvarSetFloat(sv_stopspeed->name, 75);
    }

    auto TeamGameRules::GetGameDescription() -> const char* {
        return MainConfig.getServerBrowswerInfo().c_str();
    }

    auto TeamGameRules::ReadMultiplayCvars() -> void {
        //limit_teams = 0;
        //unbalanced_rounds = 0;
        //CvarSetFloat(mp_limitteams->name, 0);
        //CvarSetFloat(mp_autoteambalance->name, 0);
        round_time = std::clamp(3 * 60, 0, 30000);
        CheckRoundLimits();
    }

    auto TeamGameRules::CheckRoundLimits() -> void {
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

    auto TeamGameRules::RestartRound() -> void {
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
            setTimer(10);
        }

        Environment.reset();

        GameRulesApi.RoundStart(isReset());
        setReset(false);
    }

    auto TeamGameRules::Think() -> void {
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
            setTimer(getTimer() - 1);
        }
        if (getTimer() != 0) {
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
                                setTimer(mode.getRoundTime());
                            } else {
                                setTimer(round_time);
                            }
                            const auto target = mode.executeLaunch();
                            ModesApi.ModeStart(modeId, target);
                            setRoundState(RoundState::Playing);
                        }
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

    auto TeamGameRules::getRandomMode() const -> int {
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

    auto TeamGameRules::EndRound(EndRoundEvent event) -> void {
        switch (event) {
            case EndRoundEvent::WarmupEnd: {
                setReset(true);
                // setGameState(GameState::Playing);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setTimer(3);
                break;
            }
            case EndRoundEvent::GameCommence: {
                setReset(true);
                setGameState(GameState::Playing);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setTimer(3);
                break;
            }
            case EndRoundEvent::GameRestart: {
                CvarSetFloat(sv_restart->name, 0);
                setReset(true);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setTimer(1);
                break;
            }
            case EndRoundEvent::GameOver: {
                setGameState(GameState::Over);
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Draw);
                setTimer(10);
                break;
            }
            case EndRoundEvent::HumansWin: {
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Cts);
                setTeamWins(Team::Human, getTeamWins(Team::Human) + 1);
                setTimer(static_cast<int>(mp_round_restart_delay->value));
                CheckRoundsLimit();
                break;
            }
            case EndRoundEvent::ZombiesWin: {
                setRoundState(RoundState::Terminate);
                setWinStatus(WinStatus::Terrorists);
                setTeamWins(Team::Zombie, getTeamWins(Team::Zombie) + 1);
                setTimer(static_cast<int>(mp_round_restart_delay->value));
                CheckRoundsLimit();
                break;
            }
        }
        GameRulesApi.RoundEnd(event, getMode(), getTimer());
    }

    auto TeamGameRules::UpdateTeamScores() -> void {
        sendTeamScore(TO_ALL, Team::Human, getTeamWins(Team::Human));
        sendTeamScore(TO_ALL, Team::Zombie, getTeamWins(Team::Zombie));
    }

    auto TeamGameRules::GoToIntermission() -> void {
        if (getGameState() == GameState::Over) {
            return;
        }
        int chatTime = std::clamp(static_cast<int>(mp_chattime->value), 0, MAX_INTERMISSION_TIME);
        setGameState(GameState::Over); // endRound?
        setTimer(chatTime);
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

    auto TeamGameRules::ChangeLevel() -> void {
        const char* nextMap = "de_dust2";
        metamod::engine::ChangeLevel(nextMap, nullptr);
    }

    auto TeamGameRules::CleanUpMap() -> void {
        OriginalGameRules->CsGameRules()->CleanUpMap();
    }

    auto TeamGameRules::RemoveGuns() -> void {
        OriginalGameRules->CsGameRules()->RemoveGuns();
    }

    auto TeamGameRules::CheckWinConditions() -> void {
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

    auto TeamGameRules::CheckRoundsLimit() -> bool {
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

    auto TeamGameRules::changeGameState(GameState gameState) -> void {
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

    auto TeamGameRules::GetTimeLeft() const {
        return time_limit_ - g_global_vars->time;
    }

    auto TeamGameRules::GetRoundElapsedTime() const {
        return g_global_vars->time - round_start_time;
    }

    auto TeamGameRules::GetMapElapsedTime() const {
        return g_global_vars->time;
    }

    auto TeamGameRules::GetRoundRespawnTime() const {
        return mp_roundrespawn_time->value;
    }

    auto TeamGameRules::getTeamWins(Team team) const -> short {
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

    auto TeamGameRules::setTeamWins(Team team, int wins, bool update) -> void {
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

    auto TeamGameRules::getWinStatus() const -> WinStatus {
        return round_win_status;
    }

    auto TeamGameRules::setWinStatus(WinStatus winStatus) -> void {
        round_win_status = winStatus;
    }

    auto TeamGameRules::isReset() const -> bool {
        return complete_reset;
    }

    auto TeamGameRules::setReset(bool isReset) -> void {
        complete_reset = isReset;
    }

    auto TeamGameRules::getRoundsPlayed() const -> int {
        return total_rounds_played;
    }

    auto TeamGameRules::setRoundsPlayed(int roundsPlayed) -> void {
        total_rounds_played = roundsPlayed;
    }

    auto TeamGameRules::getTimer() const -> int {
        return timer_;
    }

    auto TeamGameRules::setTimer(int timer) -> void {
        if (timer < 0) {
            timer = 0;
        }
        timer_ = timer;
        nextRoundTimeUpdateTime_ = g_global_vars->time + 1.0f;
        GameRulesApi.RoundTimer(getTimer());
    }

    auto TeamGameRules::getGameState() const -> GameState {
        return gameState_;
    }

    auto TeamGameRules::setGameState(GameState gameState) -> void {
        const auto oldState = gameState_;
        gameState_ = gameState;
        GameRulesApi.GameStateChanged(oldState, gameState);
    }

    auto TeamGameRules::getNextGameState() const -> GameState {
        return nextGameState_;
    }

    auto TeamGameRules::setNextGameState(GameState gameState) -> void {
        nextGameState_ = gameState;
    }

    auto TeamGameRules::getRoundState() const -> RoundState {
        return roundState_;
    }

    auto TeamGameRules::setRoundState(RoundState roundState) -> void {
        const auto oldState = roundState_;
        roundState_ = roundState;
        GameRulesApi.RoundStateChanged(oldState, roundState);
    }

    auto TeamGameRules::getNextRoundState() const -> RoundState {
        return roundState_;
    }

    auto TeamGameRules::setNextRoundState(RoundState roundState) -> void {
        nextRoundState_ = roundState;
    }

    auto TeamGameRules::getMode() const -> int {
        return modeId_;
    }

    auto TeamGameRules::setMode(int modeId) -> void {
        modeId_ = modeId;
    }

    auto TeamGameRules::getLastMode() const -> int {
        return lastModeId_;
    }

    auto TeamGameRules::setLastMode(int lastModeId) -> void {
        lastModeId_ = lastModeId;
    }

    auto TeamGameRules::getDefaultPlayerClass(Team team) const -> int {
        return defaultPlayerClass_[toInt(team)];
    }

    auto TeamGameRules::setDefaultPlayerClass(Team team, int playerClass) -> void {
        defaultPlayerClass_[toInt(team)] = playerClass;
    }

    auto TeamGameRules::getDefaultPlayerClassOverride(Team team) const -> int {
        return defaultPlayerClassOverride_[toInt(team)];
    }

    auto TeamGameRules::setDefaultPlayerClassOverride(Team team, int playerClass) -> void {
        defaultPlayerClassOverride_[toInt(team)] = playerClass;
    }

    auto TeamGameRules::defaultPlayerClass(Team team) const -> int {
        if (!getDefaultPlayerClassOverride(team)) {
            return getDefaultPlayerClass(team);
        }
        return getDefaultPlayerClassOverride(team);
    }

    auto TeamGameRules::getRespawnTeam() const -> Team {
        if (getGameState() != GameState::Playing) {
            return Team::Human;
        }
        if (getRoundState() != RoundState::Playing) {
            return Team::Human;
        }
        const auto modeRef = Modes[getMode()];
        if (modeRef) {
            const auto& mode = modeRef->get();
            switch (mode.getRespawn()) {
                case RespawnType::ToZombiesTeam: {
                    return Team::Zombie;
                }
                case RespawnType::Balance: {
                    if (getPlayersCount(PlayersCountFlags::Humans) >= getPlayersCount(PlayersCountFlags::Zombies)) {
                        return Team::Zombie;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
        return Team::Human;
    }

    auto TeamGameRules::isCanMove() const -> bool {
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
            default: {
                break;
            }
        }
        return true;
    }

    auto TeamGameRules::precache() -> void {
        shadowSprite_ = PrecacheModel("sprites/shadow_circle.spr");
    }
}
