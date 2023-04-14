#include "rezombie/convars.h"
#include <metamod/engine.h>

namespace rz
{
    using namespace metamod::engine;

    auto ConVarInit() -> void {
        sv_accelerate = CvarGetPointer("sv_accelerate");
        sv_friction = CvarGetPointer("sv_friction");
        sv_stopspeed = CvarGetPointer("sv_stopspeed");

        sv_restartround = CvarGetPointer("sv_restartround");
        sv_restart = CvarGetPointer("sv_restart");

        mp_limitteams = CvarGetPointer("mp_limitteams");
        mp_autoteambalance = CvarGetPointer("mp_autoteambalance");
        mp_timelimit = CvarGetPointer("mp_timelimit");
        mp_fadetoblack = CvarGetPointer("mp_fadetoblack");
        mp_flashlight = CvarGetPointer("mp_flashlight");
        mp_timeleft = CvarGetPointer("mp_timeleft");
        mp_forcecamera = CvarGetPointer("mp_forcecamera");
        mp_forcechasecam = CvarGetPointer("mp_forcechasecam");
        mp_maxrounds = CvarGetPointer("mp_maxrounds");
        mp_winlimit = CvarGetPointer("mp_winlimit");
        mp_chattime = CvarGetPointer("mp_chattime");
        mp_roundrespawn_time = CvarGetPointer("mp_roundrespawn_time");
        mp_round_restart_delay = CvarGetPointer("mp_round_restart_delay");
        mp_falldamage = CvarGetPointer("mp_falldamage");
    }
}
