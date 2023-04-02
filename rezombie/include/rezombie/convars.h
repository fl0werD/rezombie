#pragma once

#include <cssdk/common/cvar.h>

namespace rz
{
    using namespace cssdk;

    inline CVar* sv_accelerate = nullptr;
    inline CVar* sv_friction = nullptr;
    inline CVar* sv_stopspeed = nullptr;

    inline CVar* sv_restart = nullptr;
    inline CVar* sv_restartround = nullptr;

    inline CVar* mp_limitteams = nullptr;
    inline CVar* mp_autoteambalance = nullptr;
    inline CVar* mp_timelimit = nullptr;
    inline CVar* mp_fadetoblack = nullptr;
    inline CVar* mp_flashlight = nullptr;
    inline CVar* mp_timeleft = nullptr;
    inline CVar* mp_forcecamera = nullptr;
    inline CVar* mp_forcechasecam = nullptr;
    inline CVar* mp_maxrounds = nullptr;
    inline CVar* mp_winlimit = nullptr;
    inline CVar* mp_chattime = nullptr;
    inline CVar* mp_roundrespawn_time = nullptr;
    inline CVar* mp_round_restart_delay = nullptr;
    inline CVar* mp_falldamage = nullptr;

    auto ConVarInit() -> void;
}
