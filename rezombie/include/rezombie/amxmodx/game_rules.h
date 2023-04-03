#pragma once

#include "rezombie/core/amxx_feature.h"
#include "rezombie/gamerules/game_rules.h"
#include <array>

namespace rz {
    enum class GameModeVars : int {
        Handle,
        Name,
        DropChance,
        MinimumPlayers,
        RoundTime,
    };

    enum class GameRulesForward : int {
        ROUND_START,
        ROUND_END,
        ROUND_HUD_TIMER,
        GAME_STATE_CHANGED,
        ROUND_STATE_CHANGED,
        GAME_MODE_START,
        MAX_GAME_RULES_FORWARDS,
    };

    class AmxxGameRules : public AmxxFeature<GameRulesForward, GameRulesForward::MAX_GAME_RULES_FORWARDS> {
      public:
        AmxxGameRules() : AmxxFeature("game_rules") {}

        auto RegisterForwards() -> void override;

        auto RegisterNatives() -> void override;

        auto RoundStart(bool isReset) -> void;

        auto RoundEnd(EndRoundEvent event, int gameModeIndex, int delay) -> void;

        auto RoundHudTimer(int time) -> void;

        auto GameStateChanged(GameState oldGameState, GameState newGameState) -> void;

        auto RoundStateChanged(RoundState oldRoundState, RoundState newRoundState) -> void;

        auto GameModeStart(int gameModeIndex) -> void;
    };

    inline AmxxGameRules amxxGameRules;
}
