#include "rezombie/amxmodx/melee.h"
#include "rezombie/core/amxx_features_store.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/modules/player_model.h"
#include "rezombie/modules/weapon.h"
#include "rezombie/player/players.h"
#include "rezombie/entity/weather.h"
#include "rezombie/weapons/weapons.h"
#include <core/amxx_access.h>
#include <metamod/utils.h>
#include <mhooks/amxx.h>
#include <mhooks/metamod.h>
#include <mhooks/reapi.h>

using namespace cssdk;
using namespace core;
using namespace metamod;
using namespace mhooks;
using namespace vhooks;
using namespace message;

namespace rz
{
    auto Spawn_Pre(const GameDllDispatchSpawnMChain& chain, Edict* entity) -> int {
        if (str::Equals(entity->vars.class_name.CStr(), "armoury_entity")) {
            RETURN_META_VALUE(Result::Supercede, -1);
        }
        return chain.CallNext(entity);
    }

    auto setWeather() -> void {
        g_weather.setSkyName("space");
        g_weather.setLight("d");
        g_weather.setFogColor(Color24{127, 127, 0});
        g_weather.setFogDensity(0.0005f);
        g_weather.setWeatherType(WeatherType::Snow);

        // precache
        const auto& skyName = g_weather.getSkyName();
        if (!skyName.empty()) {
            CvarSetString("sv_skyname", skyName.c_str());
            CvarSetFloat("sv_skycolor_r", 0);
            CvarSetFloat("sv_skycolor_g", 0);
            CvarSetFloat("sv_skycolor_b", 0);
        }
    }

    auto ServerActivate_Post(const GameDllServerActivateMChain& chain, Edict* edictList, int edictCount, int clientMax)
    -> void {
        chain.CallNext(edictList, edictCount, clientMax);
        gameRules->precache();
        // previewPrecache();
        message::initMessagesId();
        VirtualHook::RegisterHooks();
        ModulesStore::precache();
        setWeather();
    }

    auto ServerDeactivate_Pre(const GameDllServerDeactivateMChain& chain) -> void {
        VirtualHook::UnregisterHooks();
        ModulesStore::clear();
        chain.CallNext();
    }

    auto UpdateClientData_Post(
        const GameDllUpdateClientDataMChain& chain, const Edict* client, qboolean sendWeapons, ClientData* data
    ) -> void {
        chain.CallNext(client, sendWeapons, data);
        const auto& player = player::players[client];
        const auto activeItem = player.getActiveItem();
        if (activeItem != nullptr) {
            const auto weaponRef = weaponModule[activeItem->vars->impulse];
            if (weaponRef) {
                data->next_attack = 1.0f;
            }
        }
    }

    auto DropClient_Post(const ReHldsDropClientMChain& chain, IGameClient* client, bool crash, const char* reason)
    -> void {
        players[client].disconnect();
        chain.CallNext(client, crash, reason);
    }

    auto OnAmxxPluginsLoaded(const AmxxPluginsLoadedMChain& chain) -> void {
        AmxxFeaturesStore::RegisterForwards();
        chain.CallNext();
    }

    auto OnAmxxAttach(const AmxxAttachMChain& chain) -> AmxxStatus {
        auto rehldsEnabled = false;
        auto status = AmxxStatus::Failed;
        if ((!rehldsEnabled || rehlds_api::Init()) && regamedll_api::Init()) {
            amxx_access::Init();
            type_conversion::Init();
            status = chain.CallNext();
        }
        if (status != AmxxStatus::Ok) {
            return status;
        }
        MHookAmxxPluginsLoaded(DELEGATE_ARG<OnAmxxPluginsLoaded>);
        MHookGameDllDispatchSpawn(DELEGATE_ARG<Spawn_Pre>, false);
        MHookGameDllServerActivate(DELEGATE_ARG<ServerActivate_Post>, true);
        MHookGameDllServerDeactivate(DELEGATE_ARG<ServerDeactivate_Pre>, false);
        // MHookGameDllAddToFullPack(DELEGATE_ARG<AddToFullPack_Pre / Post>, );
        MHookGameDllUpdateClientData(DELEGATE_ARG<UpdateClientData_Post>, true);
        if (rehldsEnabled) {
            MHookReHldsDropClient(DELEGATE_ARG<DropClient_Post>);
        }
        RegisterGameRulesHooks();
        player::RegisterHooks();
        weapon::RegisterHooks();
        AmxxFeaturesStore::restore();
        AmxxFeaturesStore::RegisterNatives();
        return status;
    }
}

auto Main() -> void {
    MHookAmxxAttach(DELEGATE_ARG<rz::OnAmxxAttach>);
}
