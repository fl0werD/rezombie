#pragma once

#include "rezombie/core/amxx_feature.h"
#include "rezombie/gamerules/game_rules.h"
#include <array>

namespace rz
{
    enum class GameModeVars : int {
        Handle,
        Name,
        DropChance,
        MinimumPlayers,
        RoundTime,
    };

    // to lower case
    enum class GameRulesForward : int {
        RoundStart,
        RoundEnd,
        RoundTimer,
        GAME_STATE_CHANGED,
        ROUND_STATE_CHANGED,
        GAME_MODE_START,
        MAX_FORWARDS,
    };

    class AmxxGameRules : public AmxxFeature<GameRulesForward, GameRulesForward::MAX_FORWARDS> {
      public:
        AmxxGameRules() : AmxxFeature("game_rules") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto RoundStart(bool isReset) -> void;

        auto RoundEnd(EndRoundEvent event, int gameModeIndex, int delay) -> void;

        auto RoundTimer(int time) -> void;

        auto GameStateChanged(GameState oldGameState, GameState newGameState) -> void;

        auto RoundStateChanged(RoundState oldRoundState, RoundState newRoundState) -> void;

        auto GameModeStart(int gameModeIndex) -> void;
    };

    inline AmxxGameRules amxxGameRules;
}
