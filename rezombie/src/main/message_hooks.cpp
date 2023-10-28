#include "rezombie/main/message_hooks.h"
#include "rezombie/messages/user_message.h"
#include <mhooks/messages.h>

namespace rz
{
    using namespace core;

    auto RegisterMessageHooks() -> void {
        MHookNetworkMessage("HideWeapon", DELEGATE_ARG<MessageHideWeapon>);
    }

    auto MessageHideWeapon(
        const NetworkMessageMChain&,
        const MessageType&,
        int,
        const float*,
        Edict* client,
        MessageArgs& args
    ) -> bool {
        //auto& flags = args.GetInteger(0);
        //flags |= HIDE_HUD_TIMER;
        sendHideWeapon(client, args.GetInteger(0) | HIDE_HUD_TIMER);
        return true;
    }
}
