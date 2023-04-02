#pragma once

#include "cssdk/common/vector.h"

namespace rz
{
    const auto VECTOR_ZERO = cssdk::Vector(0, 0, 0);

    enum class GameState : int
    {
        Warmup,
        NeedPlayers,
        Playing,
        Over,
    };

    enum class RoundState : int
    {
        None,
        Prepare,
        Playing,
        Terminate,
    };

    enum class EndRoundEvent : int
    {
        None,
        WarmupEnd,
        GameCommence,
        GameRestart,
        GameOver,
        HumansWin,
        ZombiesWin,
        EndDraw,
    };
}
