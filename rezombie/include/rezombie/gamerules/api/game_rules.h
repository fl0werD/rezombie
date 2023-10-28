#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include "rezombie/gamerules/game_rules.h"

namespace rz
{
    // to lower case
    enum class GameRulesForward : int {
        RoundStart,
        RoundEnd,
        RoundTimer,
        GameStateChanged,
        RoundStateChanged,
        MAX_FORWARDS,
    };

    class AmxxGameRules : public AmxxFeature<GameRulesForward, GameRulesForward::MAX_FORWARDS> {
      public:
        AmxxGameRules() : AmxxFeature("game_rules") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto RoundStart(bool isReset) -> void;

        auto RoundEnd(EndRoundEvent event, int modeId, int delay) -> void;

        auto RoundTimer(int time) -> void;

        auto GameStateChanged(GameState oldGameState, GameState newGameState) -> void;
        auto RoundStateChanged(RoundState oldRoundState, RoundState newRoundState) -> void;
    };

    inline AmxxGameRules GameRulesApi;
}
