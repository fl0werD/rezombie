#include "rezombie/core/api/amxx_features_store.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/main/engine_hooks.h"
#include "rezombie/main/gamedll_hooks.h"
#include "rezombie/main/message_hooks.h"
#include "rezombie/main/main.h"
#include "rezombie/configs/main_config.h"
#include <core/amxx_access.h>
#include <mhooks/amxx.h>
#include <mhooks/reapi.h>

auto Main() -> void {
    mhooks::MHookAmxxAttach(core::DELEGATE_ARG<rz::OnAmxxAttach>);
}

namespace rz
{
    using namespace mhooks;

    auto OnAmxxAttach(const AmxxAttachMChain& chain) -> AmxxStatus {
        auto status = AmxxStatus::Failed;
        if (rehlds_api::Init() && regamedll_api::Init()) {
            amxx_access::Init();
            type_conversion::Init();
            status = chain.CallNext();
        }
        if (status != AmxxStatus::Ok) {
            return status;
        }
        MHookAmxxPluginsLoaded(DELEGATE_ARG<OnAmxxPluginsLoaded>);
        RegisterEngineHooks();
        RegisterGameDllHooks();
        RegisterGameRulesHooks();
        RegisterMessageHooks();
        RegisterPlayerHooks();
        ApiStore.registerNatives();
        MainConfig.load();
        return status;
    }

    auto OnAmxxPluginsLoaded(const AmxxPluginsLoadedMChain& chain) -> void {
        ApiStore.registerForwards();
        chain.CallNext();
    }
}
