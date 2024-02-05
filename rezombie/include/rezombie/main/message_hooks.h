#pragma once

#include <mhooks/messages.h>

namespace rz
{
    using namespace cssdk;
    using namespace mhooks;

    auto RegisterMessageHooks() -> void;

    auto MessageHideWeapon(
        const NetworkMessageMChain& chain,
        const MessageType& type,
        int id,
        const float* origin,
        Edict* client,
        MessageArgs& args
    ) -> bool;

    auto MessageScreenFade(
        const NetworkMessageMChain& chain,
        const MessageType& type,
        int id,
        const float* origin,
        Edict* client,
        MessageArgs& args
    ) -> bool;
}
