#pragma once

#include <mhooks/metamod/gamedll.h>

namespace rz
{
    using namespace cssdk;

    inline auto IsServerActive = false;

    auto RegisterGameDllHooks() -> void;

    auto Spawn_Pre(
        const GameDllDispatchSpawnMChain& chain,
        Edict* entity
    ) -> int;

    auto ClientPutInServer_Post(
        const GameDllClientPutInServerMChain& chain,
        Edict* client
    ) -> void;

    auto ClientKill_Pre(
        const GameDllClientKillMChain& chain,
        Edict* client
    ) -> void;

    auto ServerActivate_Post(
        const GameDllServerActivateMChain& chain,
        Edict* edictList,
        int edictCount,
        int clientMax
    ) -> void;

    auto ServerDeactivate_Pre(
        const GameDllServerDeactivateMChain& chain
    ) -> void;

    auto AddToFullPack_Pre(
        const GameDllAddToFullPackMChain& chain,
        EntityState* state,
        int entityId,
        Edict* entity,
        Edict* host,
        int hostFlags,
        qboolean isPlayer,
        unsigned char* set
    ) -> qboolean;

    auto AddToFullPack_Post(
        const GameDllAddToFullPackMChain& chain,
        EntityState* state,
        int entityId,
        Edict* entity,
        Edict* host,
        int hostFlags,
        qboolean isPlayer,
        unsigned char* set
    ) -> qboolean;

    auto UpdateClientData_Post(
        const GameDllUpdateClientDataMChain& chain,
        const Edict* client,
        qboolean sendWeapons,
        ClientData* data
    ) -> void;

    auto GetWeaponData_Post(
        const GameDllGetWeaponDataMChain& chain,
        Edict* client,
        WeaponData* data
    ) -> qboolean;
}
