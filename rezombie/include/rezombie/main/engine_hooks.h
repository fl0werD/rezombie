#pragma once

#include <mhooks/reapi.h>

namespace rz
{
    auto RegisterEngineHooks() -> void;

    auto DropClient(
        ReHookSvDropClient* chain,
        IGameClient* client,
        bool crash,
        const char* reason
    ) -> void;

    auto WriteFullClientUpdate(
        ReHookSvWriteFullClientUpdate* chain,
        IGameClient* client,
        char* info,
        std::size_t maxLength,
        SizeBuf* infoBuffer,
        IGameClient* receiver
    ) -> void;
}
