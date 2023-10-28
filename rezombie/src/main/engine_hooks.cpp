#include "rezombie/player/players.h"
#include "rezombie/main/engine_hooks.h"
#include <mhooks/reapi.h>

namespace rz
{
    auto RegisterEngineHooks() -> void {
        auto hooks = rehlds_api::HookChains();

        hooks->SvDropClient()->RegisterHook(&DropClient);
        hooks->SvWriteFullClientUpdate()->RegisterHook(&WriteFullClientUpdate);
    }

    auto DropClient(
        ReHookSvDropClient* chain,
        IGameClient* client,
        bool crash,
        const char* reason
    ) -> void {
        chain->CallNext(client, crash, reason);
        Players[client].disconnect();
    }

    auto WriteFullClientUpdate(
        ReHookSvWriteFullClientUpdate* chain,
        IGameClient* client,
        char* info,
        std::size_t maxLength,
        SizeBuf* infoBuffer,
        IGameClient* receiver
    ) -> void {
        SetClientKeyValue(client->GetId() + 1, info, "model", "");
        chain->CallNext(client, info, maxLength, infoBuffer, receiver);
    }
}
